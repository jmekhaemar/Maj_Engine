#include "MemoryChunk.h"

#include "common/Logging.h"
#include "Device.h"

namespace maj::rhi
{
	MemoryChunk::MemoryChunk(const uint32_t kMemoryType, const uint64_t kMemorySize)
		: mMemorySize{ kMemorySize }
	{
		VkMemoryAllocateInfo allocInfo{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
		allocInfo.allocationSize = mMemorySize;
		allocInfo.memoryTypeIndex = kMemoryType;

		VkResult result{ vkAllocateMemory(Service::get<Device>()->device(), &allocInfo, nullptr, &mMemory) };
		if (result != VK_SUCCESS)
			THROW("failed to allocate memory with error: " + std::to_string(result))

		mMemoryBlocks.emplace_back(new MemoryBlock{ kMemorySize, 0u, true });
	}

	MemoryChunk::~MemoryChunk()
	{
		vkFreeMemory(Service::get<Device>()->device(), mMemory, nullptr);
	}

	MemoryInfo MemoryChunk::allocate(const uint64_t kMemorySize)
	{
		if ((memoryUsedSize() + kMemorySize) > mMemorySize)
			return { 0u, nullptr, nullptr };

		for (std::vector<std::shared_ptr<MemoryBlock>>::iterator it { mMemoryBlocks.begin() } ; it != mMemoryBlocks.end() ; ++it)
		{
			std::shared_ptr<MemoryBlock> block{ (*it) };
			if (block->isMemoryFree() && block->memorySize() > kMemorySize)
			{
				mMemoryBlocks.emplace(it + 1, new MemoryBlock { block->memorySize() - kMemorySize, block->memoryOffset() + kMemorySize, true });
				*block = { kMemorySize, block->memoryOffset() };
				merge();
				return { 0u, this, &(*block) };
			}
		}

		return { 0u, nullptr, nullptr };
	}

	void MemoryChunk::free(const MemoryInfo& kMemoryInfo)
	{
		kMemoryInfo.mMemoryBlock->free();
		merge();
	}

	uint64_t MemoryChunk::memorySize() const
	{
		return mMemorySize;
	}

	uint64_t MemoryChunk::memoryUsedSize() const
	{
		uint64_t memoryUsedSize { 0u };

		for (const std::shared_ptr<MemoryBlock>& block : mMemoryBlocks)
			if (!block->isMemoryFree())
				memoryUsedSize += block->memorySize();

		return memoryUsedSize;
	}

	VkDeviceMemory MemoryChunk::memory() const
	{
		return mMemory;
	}

	JsonNode* MemoryChunk::json() const
	{
		JsonNode* root{ new JsonNode{ JsonNode::EType::OBJECT } };

		root->addNode("memoryUsedSize", new JsonNode{ MemoryBlock::byteHumanReadable(memoryUsedSize()) });
		root->addNode("memorySize", new JsonNode{ MemoryBlock::byteHumanReadable(memorySize()) });
		root->addNode("memoryBlocks", new JsonNode{ JsonNode::EType::ARRAY });

		uint32_t memoryBlockCount { static_cast<uint32_t>(mMemoryBlocks.size()) };
		for (uint32_t i{ 0u }; i < memoryBlockCount; ++i)
			(*root)["memoryBlocks"]->addChild(mMemoryBlocks[i]->json());

		return root;
	}

	void MemoryChunk::merge()
	{
		for (std::vector<std::shared_ptr<MemoryBlock>>::iterator it { mMemoryBlocks.begin() }; it != mMemoryBlocks.end(); ++it)
		{
			std::shared_ptr<MemoryBlock>& currentBlock { *it };
			if (currentBlock->isMemoryFree())
			{
				std::shared_ptr<MemoryBlock> nextBlock {};
				while(it + 1 != mMemoryBlocks.end())
				{
					nextBlock = { *(it + 1) };
					if (nextBlock->isMemoryFree())
					{
						*currentBlock = { currentBlock->memorySize() + nextBlock->memorySize(), currentBlock->memoryOffset(), true };
						mMemoryBlocks.erase(it + 1);
					}
					else
						break;
				}
			}
		}
	}
}