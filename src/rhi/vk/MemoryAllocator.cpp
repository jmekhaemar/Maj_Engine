#include "MemoryAllocator.h"

#include "common/Logging.h"
#include "Device.h"

namespace maj::rhi
{
	MemoryAllocator::MemoryAllocator()
		: Service{ typeid(MemoryAllocator).name() }
	{
		const Device* kDevice { Service::get<Device>() };

		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(kDevice->physicalDevice(), &memoryProperties);

		mPageSize = kDevice->physicalDeviceProperties().limits.bufferImageGranularity;

		uint32_t maxMemoryAvailable { 0u };
		for (uint32_t i{ 0u }; i < memoryProperties.memoryHeapCount; ++i)
			maxMemoryAvailable += static_cast<uint32_t>(memoryProperties.memoryHeaps[i].size);

		mChunkSize = maxMemoryAvailable / kDevice->physicalDeviceProperties().limits.maxMemoryAllocationCount;
		if (64 * 1024 * 1024 > mChunkSize)
			mChunkSize = 64 * 1024 * 1024;
		mChunkSize += mPageSize - (mChunkSize % mPageSize);

		mMemoryPools.reserve(memoryProperties.memoryTypeCount);
		for (uint8_t i{ 0u }; i < memoryProperties.memoryTypeCount; ++i)
			mMemoryPools.emplace_back(i, mChunkSize);
	}

	MemoryAllocator::~MemoryAllocator()
	{}

	const MemoryInfo MemoryAllocator::allocate(const VkMemoryAllocateInfo& kRequirements)
	{
		uint64_t memorySize { kRequirements.allocationSize + mPageSize - (kRequirements.allocationSize % mPageSize) };
		return mMemoryPools[kRequirements.memoryTypeIndex].allocate(memorySize);
	}

	void MemoryAllocator::free(const MemoryInfo& kMemoryInfo)
	{
		mMemoryPools[kMemoryInfo.mMemoryType].free(kMemoryInfo);
	}

	uint64_t MemoryAllocator::chunkSize() const
	{
		return mChunkSize;
	}

	void MemoryAllocator::dump() const
	{
		JsonNode* root { json() };
		root->save("memory_dump.json");

		if (root)
			delete root;

		LOG(LogRHIInfo, "memory dump was generated")
	}

	JsonNode* MemoryAllocator::json() const
	{
		JsonNode* root { new JsonNode{ JsonNode::EType::OBJECT } };
		root->addNode("pageSize", new JsonNode { mPageSize });
		root->addNode("chunkSize", new JsonNode { mChunkSize });
		root->addNode("memoryPools", new JsonNode { JsonNode::EType::ARRAY });

		uint8_t memoryPoolCount { static_cast<uint8_t>(mMemoryPools.size()) };
		for (uint8_t i{ 0u }; i < memoryPoolCount ; ++i)
			(*root)["memoryPools"]->addChild(mMemoryPools[i].json());

		return root;
	}
}