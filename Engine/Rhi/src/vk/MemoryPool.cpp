#include "MemoryPool.h"

namespace maj::rhi
{
	MemoryPool::MemoryPool(const uint32_t kMemoryType, const uint64_t kChunkSize)
		: mkMemoryType{ kMemoryType }, mkChunkSize{ kChunkSize }
	{}

	MemoryPool::MemoryPool(MemoryPool&& other)
		: mkMemoryType { other.mkMemoryType }, mkChunkSize { other.mkChunkSize }, mMemoryChunks { std::move(other.mMemoryChunks) }
	{}

	const MemoryInfo MemoryPool::allocate(const uint64_t kMemorySize)
	{
		MemoryInfo memoryInfo{ mkMemoryType, nullptr, nullptr };
		for (const std::shared_ptr<MemoryChunk>& chunk : mMemoryChunks)
		{
			memoryInfo = chunk->allocate(kMemorySize);
			memoryInfo.mMemoryType = mkMemoryType;
			if (memoryInfo.mMemoryChunk != nullptr)
				return memoryInfo;
		}
		
		if (kMemorySize > mkChunkSize)
			mMemoryChunks.emplace_back(new MemoryChunk{ mkMemoryType, mkChunkSize * ((kMemorySize / mkChunkSize) + 1) });
		else
			mMemoryChunks.emplace_back(new MemoryChunk{ mkMemoryType, mkChunkSize });

		memoryInfo = mMemoryChunks.back()->allocate(kMemorySize);
		memoryInfo.mMemoryType = mkMemoryType;
		return memoryInfo;
	}

	void MemoryPool::free(const MemoryInfo& kMemoryInfo)
	{
		kMemoryInfo.mMemoryChunk->free(kMemoryInfo);

		for (std::vector<std::shared_ptr<MemoryChunk>>::iterator it { mMemoryChunks.begin() }; it != mMemoryChunks.end(); it++)
		{
			if ((*it)->memoryUsedSize() == 0u)
			{
				mMemoryChunks.erase(it);
				break;
			}
		}
	}

	uint32_t MemoryPool::memoryType() const
	{
		return mkMemoryType;
	}

	uint64_t MemoryPool::memorySize() const
	{
		uint64_t memorySize { 0u };
		for (const std::shared_ptr<MemoryChunk>& chunk : mMemoryChunks)
			memorySize += chunk->memorySize();

		return memorySize;
	}

	uint64_t MemoryPool::memoryUsedSize() const
	{
		uint64_t memoryUsedSize{ 0u };
		for (const std::shared_ptr<MemoryChunk>& chunk : mMemoryChunks)
			memoryUsedSize += chunk->memoryUsedSize();

		return memoryUsedSize;
	}

	JsonNode* MemoryPool::json() const
	{
		JsonNode* root{ new JsonNode{ JsonNode::EType::OBJECT } };

		root->addNode("memoryType", new JsonNode{ mkMemoryType });
		root->addNode("chunkSize", new JsonNode{ MemoryBlock::byteHumanReadable(mkChunkSize) });
		root->addNode("memoryUsedSize", new JsonNode{ MemoryBlock::byteHumanReadable(memoryUsedSize()) });
		root->addNode("memorySize", new JsonNode{ MemoryBlock::byteHumanReadable(memorySize()) });
		root->addNode("memoryChunks", new JsonNode{ JsonNode::EType::ARRAY });

		uint32_t memoryChunkCount { static_cast<uint32_t>(mMemoryChunks.size()) };
		for (uint32_t i{ 0u }; i < memoryChunkCount ; ++i)
			(*root)["memoryChunks"]->addChild(mMemoryChunks[i]->json());

		return root;
	}
}
