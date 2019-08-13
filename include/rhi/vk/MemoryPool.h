#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

#include "MemoryChunk.h"

namespace maj::rhi
{
	class MemoryPool final : public NonCopyable
	{
	public:
		MemoryPool(const uint32_t kMemoryType, const uint64_t kChunkSize);
		MemoryPool(MemoryPool&& other);
		~MemoryPool() = default;

		const MemoryInfo allocate(const uint64_t kMemorySize);
		void free(const MemoryInfo& kMemoryInfo);

		uint32_t memoryType() const;
		uint64_t memorySize() const;
		uint64_t memoryUsedSize() const;

		JsonNode* json() const;

	private:
		const uint32_t mkMemoryType;
		const uint64_t mkChunkSize;
		std::vector<std::shared_ptr<MemoryChunk>> mMemoryChunks;
	};
}

#endif