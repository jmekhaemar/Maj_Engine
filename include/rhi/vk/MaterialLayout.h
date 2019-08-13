#ifndef _MATERIAL_LAYOUT_H_
#define _MATERIAL_LAYOUT_H_

#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include <map>

#include <NonCopyable.h>
#include <Asset.h>

#include "common/Material.h"

namespace maj::rhi
{
	class MaterialLayout final : public NonCopyable, public ISerializable
	{
		ENABLE_META_CUSTOM(MaterialLayout)

	public:
		typedef std::map<uint32_t, LayoutSet> SetMap;

		struct Layout final : NonCopyable
		{
		public:
			Layout();
			Layout(const std::map<uint32_t, LayoutBinding>& kBindings);
			Layout(Layout&& other);
			~Layout();

			Layout& operator=(Layout&& other);

			VkDescriptorSetLayout mLayout;
		};

	public:
		static const uint32_t skMaxMaterialInstance;

	private:
		static void remove(const MaterialLayout* kLayout);

		static std::unordered_map<const MaterialLayout*, uint32_t> sInstanceCounts;
		static std::unordered_map<const MaterialLayout*, uint32_t> sMaxInstanceCounts;

	public:
		LOAD_CONSTRUCTOR_CUSTOM(MaterialLayout)

		MaterialLayout(SetMap&& sets, uint32_t maxInstanceCount = 0u);
		MaterialLayout(const MaterialLayout& kOther, SetMap&& sets);
		~MaterialLayout();

		bool addReference() const;
		void removeReference() const;

		const SetMap& sets() const;
		const LayoutSet* set(uint32_t index) const;
		const LayoutBinding* binding(uint32_t setindex, uint32_t bindingIndex) const;
		uint32_t bindingCount() const;

		VkDescriptorPool pool() const;
		const std::vector<Layout>& pipelineLayouts() const;
		std::vector<VkDescriptorSetLayout> descriptorLayouts() const;

		Event<void> mOnChange;

	private:
		std::vector<VkDescriptorPoolSize> descriptorPoolSizes() const;
		void createDescriptorPool(uint32_t maxInstanceCount);

		uint32_t mMaxInstanceCount;

		SetMap mSets;

		VkDescriptorPool mPool;
		std::vector<Layout> mSetLayouts;
	};
}

META_CLASS(maj::Asset<maj::rhi::MaterialLayout>, 1u)
META_FIELD(maj::Asset<maj::rhi::MaterialLayout>, uint32_t, mGuid, 0u)

META_CLASS(maj::rhi::MaterialLayout, 3u)
META_FIELD(maj::rhi::MaterialLayout, uint32_t, mGuid, 0u)
META_FIELD(maj::rhi::MaterialLayout, uint32_t, mMaxInstanceCount, 1u)
META_FIELD_MAP(maj::rhi::MaterialLayout, maj::rhi::MaterialLayout::SetMap, mSets, 2u)

#endif