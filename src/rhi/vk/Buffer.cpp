#include "Buffer.h"

#include "common/Logging.h"
#include "Device.h"
#include "Renderer.h"

namespace maj::rhi
{
	const std::unordered_map<Buffer::EType, EMemoryFlag> Buffer::skMemoryProperties
	{
		{ Buffer::EType::STAGING, EMemoryFlag::HOST_VISIBLE | EMemoryFlag::HOST_COHERENT | EMemoryFlag::HOST_CACHED },
		{ Buffer::EType::MODEL,  EMemoryFlag::DEVICE_LOCAL },
		{ Buffer::EType::UNIFORM,  EMemoryFlag::HOST_VISIBLE | EMemoryFlag::HOST_COHERENT },
		{ Buffer::EType::TEXEL,  EMemoryFlag::HOST_VISIBLE | EMemoryFlag::HOST_COHERENT }
	};

	const std::unordered_map<Buffer::EType, VkBufferUsageFlags> Buffer::skBufferUsages
	{
		{ Buffer::EType::STAGING, VK_BUFFER_USAGE_TRANSFER_SRC_BIT },
		{ Buffer::EType::MODEL, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT
		| VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT },
		{ Buffer::EType::UNIFORM, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT },
		{ Buffer::EType::TEXEL, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT }
	};

	Buffer::SubBuffer::SubBuffer(const Buffer* kBuffer, const uint64_t kSize, const uint64_t kOffset)
		: mkBuffer { kBuffer }, mkSize { kSize }, mkOffset { kOffset }, mkDescriptorInfo { mkBuffer ? mkBuffer->mBuffer : VK_NULL_HANDLE, mkOffset, mkSize }
	{}

	VkWriteDescriptorSet Buffer::SubBuffer::writeDescriptorSet(VkDescriptorSet set, uint32_t binding) const
	{
		VkWriteDescriptorSet writeSet { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
		writeSet.dstSet = set;
		writeSet.dstBinding = binding;
		writeSet.dstArrayElement = 0u;
		writeSet.descriptorCount = 1u;
		writeSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; // TODO depends on the buffer usage
		writeSet.pBufferInfo = &mkDescriptorInfo;

		return writeSet;
	}

	void Buffer::SubBuffer::transfer(const SubBuffer& kDst)
	{
		Renderer::SingleTimeCommand commandBuffer {};

		VkBufferCopy copyRegion { mkOffset, kDst.mkOffset, mkSize };
		vkCmdCopyBuffer(commandBuffer.mBuffer, mkBuffer->mBuffer, kDst.mkBuffer->mBuffer, 1, &copyRegion);
	}

	const Buffer* Buffer::SubBuffer::buffer() const
	{
		return mkBuffer;
	}

	uint64_t Buffer::SubBuffer::size() const
	{
		return mkSize;
	}

	uint64_t Buffer::SubBuffer::offset() const
	{
		return mkOffset;
	}

	Buffer::MemoryMap::MemoryMap(const SubBuffer& kBuffer)
		: mData {nullptr}, mMemoryInfo { kBuffer.buffer()->mMemoryInfo }
	{
		if (!mMemoryInfo.mMemoryChunk)
			return;

		VkResult result{ vkMapMemory(Service::get<Device>()->device(), mMemoryInfo.mMemoryChunk->memory(), mMemoryInfo.mMemoryBlock->memoryOffset() + kBuffer.offset(),
									 kBuffer.size(), 0u, reinterpret_cast<void**>(&mData)) };
		if(result != VK_SUCCESS)
			THROW("failed to map memory with error: " + std::to_string(result))
	}

	Buffer::MemoryMap::~MemoryMap()
	{
		if (!mMemoryInfo.mMemoryChunk)
			return;

		vkUnmapMemory(Service::get<Device>()->device(), mMemoryInfo.mMemoryChunk->memory());
	}

	Buffer::Handler::Handler()
		: Service { typeid(Handler).name() }, mBuffers {}
	{}

	Buffer::Handler::~Handler()
	{
		for (const std::pair<Buffer::EType, std::vector<std::shared_ptr<Buffer>>>& kBuffers : mBuffers)
		{
			uint32_t bufferSize { static_cast<uint32_t>(kBuffers.second.size()) };
			if (bufferSize > 0u)
				LOG(LogRHIWarning, "deleting buffer handler: " << bufferSize << " buffers of type " << static_cast<uint8_t>(kBuffers.first) << " still remaining")
		}
	}

	Buffer::SubBuffer* Buffer::Handler::request(EType type, uint64_t size)
	{
		const Device* kDevice { Service::get<Device>() };
		const VkPhysicalDeviceProperties& kProperties { kDevice->physicalDeviceProperties() };

		// Find size of the sub buffer
		if (type == EType::UNIFORM)
			size += kProperties.limits.minUniformBufferOffsetAlignment - (size % kProperties.limits.minUniformBufferOffsetAlignment);
		else if(type == EType::TEXEL)
			size += kProperties.limits.minTexelBufferOffsetAlignment - (size % kProperties.limits.minTexelBufferOffsetAlignment);
		else if (type == EType::STAGING)
			size += 4u - (size % 4u);

		// Search through every buffers of this type
		for (const std::shared_ptr<Buffer>& kBuffer : mBuffers[type])
		{
			// Find the first available sub buffer
			const SubBuffer* kPrevious { nullptr };
			uint64_t realOffset { 0u };

			for (std::vector<std::shared_ptr<SubBuffer>>::const_iterator it { kBuffer->mSubBuffers.begin() } ; it != kBuffer->mSubBuffers.end() ; ++it)
			{
				const SubBuffer* kCrt { it->get() };

				// If there is enough space between the previous and current sub buffers
				if (kCrt->offset() - realOffset > size)
					return kBuffer->mSubBuffers.emplace(it, new SubBuffer { kBuffer.get(), size, realOffset })->get();

				kPrevious = kCrt;
				realOffset = kPrevious->size() + kPrevious->offset();
			}

			// If there is enough space for a new sub buffer
			if (kPrevious && kBuffer->mInfo.size - realOffset > size)
				return kBuffer->mSubBuffers.emplace_back(new SubBuffer { kBuffer.get(), size, realOffset }).get();
		}

		// There is no buffer of this type
		uint64_t bufferSize { Service::get<MemoryAllocator>()->chunkSize() / 4u };
		Buffer* buffer { nullptr };
		if (size > bufferSize)
			buffer = mBuffers[type].emplace_back(new Buffer { type, bufferSize * ((size / bufferSize) + 1) }).get();
		else
			buffer = mBuffers[type].emplace_back(new Buffer { type, bufferSize }).get();

		// Create the first sub buffer
		return buffer->mSubBuffers.emplace_back(new SubBuffer { buffer, size }).get();
	}

	void Buffer::Handler::release(const SubBuffer* kSubBuffer)
	{
		if (!kSubBuffer)
			return;

		const Buffer* kBuffer { kSubBuffer->buffer() };
		EType type { kBuffer->mkType };

		// Find the buffer of the sub buffer
		for (std::vector<std::shared_ptr<Buffer>>::const_iterator it { mBuffers[type].begin() }; it != mBuffers[type].end() ; ++it)
		{
			if (it->get() != kBuffer)
				continue;

			// Find the sub buffer in the buffer
			bool found { false };
			for (std::vector<std::shared_ptr<SubBuffer>>::const_iterator subIt { kBuffer->mSubBuffers.begin() }; subIt != kBuffer->mSubBuffers.end() && !found ;)
			{
				if (subIt->get() != kSubBuffer)
				{
					++subIt;
					continue;
				}

				subIt = it->get()->mSubBuffers.erase(subIt);
			}

			// If there is no other sub buffer, remove the buffer
			if (kBuffer->mSubBuffers.empty())
				mBuffers[type].erase(it);

			return;
		}
	}

	Buffer::SubBuffer* Buffer::request(EType type, uint64_t size)
	{
		return Service::get<Buffer::Handler>()->request(type, size);
	}

	void Buffer::release(const SubBuffer* kSubBuffer)
	{
		return Service::get<Buffer::Handler>()->release(kSubBuffer);
	}

	Buffer::Buffer(uint64_t size, VkBufferUsageFlags usage, EMemoryFlag properties)
		: mkType { EType::CUSTOM }, mInfo { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO }, mBuffer { VK_NULL_HANDLE }, mMemoryInfo { 0u, nullptr, nullptr }
	{
		mInfo.size = size;
		mInfo.usage = usage;
		mInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		createBuffer(properties);
	}

	Buffer::Buffer(EType type, uint64_t size)
		: mkType{ type }, mBuffer { VK_NULL_HANDLE }, mMemoryInfo{ 0u, nullptr, nullptr }
	{
		createInfo(size);
		createBuffer(skMemoryProperties.at(mkType));
	}

	Buffer::~Buffer()
	{
		Service::get<Renderer>()->wait();

		const Device* kDevice { Service::get<Device>() };
		if (mBuffer)
			vkDestroyBuffer(kDevice->device(), mBuffer, nullptr);

		Service::get<MemoryAllocator>()->free(mMemoryInfo);
	}

	VkBuffer Buffer::buffer() const
	{
		return mBuffer;
	}

	void Buffer::createInfo(uint64_t size)
	{
		VkBufferCreateInfo info{ VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
		info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		info.size = size;
		
		if(mkType == EType::CUSTOM)
			THROW("failed to create buffer: not enough information for custom type")

		info.usage = skBufferUsages.at(mkType);
		mInfo = info;
	}

	void Buffer::createBuffer(EMemoryFlag properties)
	{
		const Device* kDevice { Service::get<Device>() };

		VkResult result { vkCreateBuffer(kDevice->device(), &mInfo, nullptr, &mBuffer) };
		if (result != VK_SUCCESS)
			THROW("failed to create buffer with error: " + std::to_string(result))

		VkMemoryRequirements memRequirements {};
		vkGetBufferMemoryRequirements(kDevice->device(), mBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = kDevice->findMemoryType(memRequirements.memoryTypeBits, properties);

		mMemoryInfo = Service::get<MemoryAllocator>()->allocate(allocInfo);

		result = vkBindBufferMemory(kDevice->device(), mBuffer, mMemoryInfo.mMemoryChunk->memory(), mMemoryInfo.mMemoryBlock->memoryOffset());
		if (result != VK_SUCCESS)
			THROW("failed to bind buffer memory with error: " + std::to_string(result))
	}
}
