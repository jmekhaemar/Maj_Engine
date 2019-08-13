#include "MemoryBlock.h"

#include <string>

namespace maj::rhi
{
	const std::array<const char*, 4u> MemoryBlock::skByteUnit
	{
		{
			"B",
			"KB",
			"MB",
			"GB"
		}
	};

	std::string MemoryBlock::byteHumanReadable(uint64_t bytes)
	{
		uint32_t i { 0u };
		while (bytes >= 1024u && i < skByteUnit.size())
		{
			bytes /= 1024u;
			++i;
		}

		return std::to_string(bytes) + skByteUnit[i];
	}

	MemoryBlock::MemoryBlock(const uint64_t kMemorySize, const uint64_t kMemoryOffset, bool isMemoryFree)
		: mMemorySize { kMemorySize }, mMemoryOffset { kMemoryOffset }, mIsMemoryFree { isMemoryFree }
	{}

	void MemoryBlock::free()
	{
		mIsMemoryFree = true;
	}

	uint64_t MemoryBlock::memorySize() const
	{
		return mMemorySize;
	}

	uint64_t MemoryBlock::memoryOffset() const
	{
		return mMemoryOffset;
	}

	bool MemoryBlock::isMemoryFree() const
	{
		return mIsMemoryFree;
	}

	JsonNode* MemoryBlock::json() const
	{
		JsonNode* root{ new JsonNode{ JsonNode::EType::OBJECT} };

		root->addNode("memorySize", new JsonNode{ std::to_string(mMemorySize) + 'B' });
		root->addNode("memoryOffset", new JsonNode{ std::to_string(mMemoryOffset) + 'B' });
		root->addNode("isMemoryFree", new JsonNode{ mIsMemoryFree });

		return root;
	}
}