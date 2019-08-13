#ifndef _MEMORY_CHUNK_H_
#define _MEMORY_CHUNK_H_

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

#include <NonCopyable.h>

#include "MemoryBlock.h"

namespace maj::rhi
{
	class MemoryChunk;

	struct MemoryInfo final
	{
		uint32_t mMemoryType;
		MemoryChunk* mMemoryChunk;
		MemoryBlock* mMemoryBlock;
	};

	class MemoryChunk final : public NonCopyable
	{
	public:
		MemoryChunk(const uint32_t kMemoryType, const uint64_t kMemorySize);
		~MemoryChunk();

		MemoryInfo allocate(const uint64_t kMemorySize);
		void free(const MemoryInfo& kMemoryInfo);

		uint64_t memorySize() const;
		uint64_t memoryUsedSize() const;
		VkDeviceMemory memory() const;

		JsonNode* json() const;

	private:
		void merge();

		VkDeviceMemory mMemory;
		uint64_t mMemorySize;
		std::vector<std::shared_ptr<MemoryBlock>> mMemoryBlocks;
	};
}

#endif