#ifndef _MEMORY_ALLOCATOR_H_
#define _MEMORY_ALLOCATOR_H_

#include <Service.h>

#include "MemoryPool.h"

namespace maj::rhi
{
	class MemoryAllocator final : public Service
	{
	public:
		MemoryAllocator();
		~MemoryAllocator();

		const MemoryInfo allocate(const VkMemoryAllocateInfo& kRequirements);
		void free(const MemoryInfo& kMemoryInfo);

		uint64_t chunkSize() const;

		void dump() const;
		JsonNode* json() const;

	private:
		uint64_t mPageSize;
		uint64_t mChunkSize;
		std::vector<MemoryPool> mMemoryPools;
	};
}

#endif