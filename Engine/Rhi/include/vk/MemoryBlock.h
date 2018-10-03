#ifndef _MEMORY_BLOCK_H_
#define _MEMORY_BLOCK_H_

#include <array>

#include <JsonNode.h>

namespace maj::rhi
{
	class MemoryBlock final
	{
	public:
		static std::string byteHumanReadable(uint64_t bytes); // TODO might want to move it in utils

		static const std::array<const char*, 4u> skByteUnit;

	public:
		MemoryBlock(const uint64_t kMemorySize, const uint64_t kMemoryOffset, bool isMemoryFree = false);
		~MemoryBlock() = default;

		void free();

		uint64_t memorySize() const;
		uint64_t memoryOffset() const;
		bool isMemoryFree() const;

		JsonNode* json() const;

	private:
		uint64_t mMemorySize;
		uint64_t mMemoryOffset;
		bool mIsMemoryFree;
	};
}

#endif