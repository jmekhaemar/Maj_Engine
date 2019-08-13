#ifndef _FRAME_GRAPH_H_
#define _FRAME_GRAPH_H_

#include <map>
#include <unordered_map>

#include <Event.h>

#include "DrawCall.h"
#include "Fence.h"

#define MATERIAL_INDEX_BIT 16u
#define SORTING_ORDER_BIT 8u
#define SUBPASS_INDEX_BIT 3u
#define RENDERPASS_INDEX_BIT 4u

namespace maj::rhi
{
	class FrameGraph final : public NonCopyable
	{
	public:
		struct Key final
		{
		public:
			static const uint32_t skMaxMaterialIndex;
			static const uint32_t skMaxSortingOrder;
			static const uint32_t skMaxSubpassIndex;
			static const uint32_t skMaxRenderPassIndex;

			operator uint32_t() const;

			uint32_t mMaterialIndex : MATERIAL_INDEX_BIT;
			uint32_t mSortingOrder : SORTING_ORDER_BIT;
			uint32_t mIsScreenSpace : 1u;
			uint32_t mSubpassIndex : SUBPASS_INDEX_BIT;
			uint32_t mRenderPassIndex : RENDERPASS_INDEX_BIT;
		};

	private:
		struct CommandBuffer final : public NonCopyable
		{
			CommandBuffer(VkCommandBuffer buffer);
			CommandBuffer(CommandBuffer&& other);
			~CommandBuffer() = default;

			VkCommandBuffer mBuffer;
			bool mIsDirty;
		};

		struct RenderPassRef final : public NonCopyable
		{
			RenderPassRef(const RenderPass& kRenderPass);
			~RenderPassRef();

			uint32_t materialIndex(const Material& kMaterial, bool addIfNone = true);

			const RenderPass* mkRenderPass;
			std::unordered_map<uint32_t, const Material*> mMaterials;
		};

		static uint32_t renderPassIndex(const RenderPass& kRenderPass, bool addIfNone = true);
		static uint32_t materialIndex(const Material& kMaterial, uint32_t renderPassIndex, bool addIfNone = true);

		static std::unordered_map<uint32_t, RenderPassRef> sRenderPasses;
		static uint32_t sMaterialCount;

	public:
		FrameGraph();
		~FrameGraph();

		void clear();

		bool isDirty() const;
		void dirty(const RenderPass& kRenderPass);
		void dirty(const Material* kMaterial);
		void dirty(const Material& kMaterial);

		void record();

		template <typename... Args>
		void add(const MaterialInstance* kMaterialInstance, Args&&... args);
		void remove(const MaterialInstance* kMaterialInstance);
		void remove(const RenderPass& kRenderPass);

		std::vector<VkCommandBuffer> commandBuffers(const RenderPass& kRenderPass, uint32_t subpassIndex) const;

		Event<void> mOnRecorded;

	private:
		void createCommands(uint32_t newCount);
		void replaceDrawCallKey(const Key& kNewKey);
		void replaceCommandBufferKey(const Key& kNewKey);

		std::multimap<Key, DrawCall> mDrawCalls;
		std::map<Key, CommandBuffer> mCommands;

		bool mIsDirty;
	};
}

#include "FrameGraph.inl"

#endif