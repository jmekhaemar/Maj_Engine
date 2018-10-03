#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "common/Memory.h"

#include "IDescriptor.h"
#include "MemoryAllocator.h"

namespace maj::rhi
{
	class Buffer final : public NonCopyable
	{
	public:
		struct SubBuffer final : public IDescriptor
		{
		public:
			SubBuffer(const Buffer* kBuffer, const uint64_t kSize = 0u, const uint64_t kOffset = 0u);

			virtual VkWriteDescriptorSet writeDescriptorSet(VkDescriptorSet set, uint32_t binding) const override;

			void transfer(const SubBuffer& kDst);

			const Buffer* buffer() const;
			uint64_t size() const;
			uint64_t offset() const;

		private:
			const Buffer* mkBuffer;
			const uint64_t mkSize;
			const uint64_t mkOffset;

			VkDescriptorBufferInfo mkDescriptorInfo;
		};

		struct MemoryMap final : public NonCopyable
		{
		public:
			MemoryMap(const SubBuffer& kBuffer);
			~MemoryMap();

			char* mData;

		private:
			MemoryInfo mMemoryInfo;
		};

		enum class EType
		{
			STAGING,
			MODEL,
			UNIFORM,
			TEXEL,
			CUSTOM
		};

		struct Handler final : public Service
		{
			Handler();
			~Handler();

			SubBuffer* request(EType type, uint64_t size);
			void release(const SubBuffer* kSubBuffer);

		private:
			std::unordered_map<EType, std::vector<std::shared_ptr<Buffer>>> mBuffers;
		};

	public:
		static SubBuffer* request(EType type, uint64_t size);
		static void release(const SubBuffer* kSubBuffer);

	private:
		const static std::unordered_map<EType, EMemoryFlag> skMemoryProperties;
		const static std::unordered_map<EType, VkBufferUsageFlags> skBufferUsages;

	public:
		Buffer(uint64_t size, VkBufferUsageFlags usage, EMemoryFlag properties);
		Buffer(EType type, uint64_t size);
		virtual ~Buffer();

		VkBuffer buffer() const;

	private:
		void createInfo(uint64_t size);
		void createBuffer(EMemoryFlag properties);

		const EType mkType;
		VkBufferCreateInfo mInfo;

		VkBuffer mBuffer;
		MemoryInfo mMemoryInfo;
		std::vector<std::shared_ptr<SubBuffer>> mSubBuffers;
	};
}

#endif