#include "MaterialLayout.h"

#include "Device.h"
#include "VkHelper.h"

#include "Renderer.h"

namespace maj::rhi
{
	DEFINE_HASH_OVERLOAD(MaterialLayout)

	const uint32_t MaterialLayout::skMaxMaterialInstance { 2048u };
	std::unordered_map<const MaterialLayout*, uint32_t> MaterialLayout::sInstanceCounts {};
	std::unordered_map<const MaterialLayout*, uint32_t> MaterialLayout::sMaxInstanceCounts {};

	void MaterialLayout::remove(const MaterialLayout* kLayout)
	{
		if (sInstanceCounts.count(kLayout) == 0u)
			return;

		if (sInstanceCounts.at(kLayout) > 0u)
			LOG(LogRHIWarning, "deleting material layout: " << sInstanceCounts.at(kLayout) << " material instance(s) of this layout remaining")
	}

	MaterialLayout::Layout::Layout()
		: mLayout { VK_NULL_HANDLE }
	{
		VkDescriptorSetLayoutCreateInfo layoutInfo { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
		layoutInfo.bindingCount = 0u;
		layoutInfo.pBindings = nullptr;

		VkResult result { vkCreateDescriptorSetLayout(Service::get<Device>()->device(), &layoutInfo, nullptr, &mLayout) };
		if (result != VK_SUCCESS)
			THROW("failed to create empty descriptor set layout with error: " + std::to_string(result))
	}

	MaterialLayout::Layout::Layout(const std::map<uint32_t, LayoutBinding>& kBindings)
		: mLayout { VK_NULL_HANDLE }
	{
		std::vector<VkDescriptorSetLayoutBinding> bindings {};
		uint32_t bindingCount { static_cast<uint32_t>(kBindings.size()) };
		bindings.reserve(bindingCount);

		for (const std::pair<const uint32_t, LayoutBinding>& kPair : kBindings)
		{
			VkDescriptorSetLayoutBinding binding {	kPair.first, toDescriptorType(kPair.second.mType), kPair.second.mCount,
													static_cast<VkShaderStageFlags>(toShaderStageFlag(kPair.second.mShaderStage)), nullptr };
			bindings.emplace_back(std::move(binding));
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
		layoutInfo.bindingCount = bindingCount;
		layoutInfo.pBindings = bindings.data();

		VkResult result { vkCreateDescriptorSetLayout(Service::get<Device>()->device(), &layoutInfo, nullptr, &mLayout) };
		if (result != VK_SUCCESS)
			THROW("failed to create descriptor set layout with error: " + std::to_string(result))
	}

	MaterialLayout::Layout::Layout(Layout&& other)
		: mLayout { other.mLayout }
	{
		other.mLayout = VK_NULL_HANDLE;
	}

	MaterialLayout::Layout::~Layout()
	{
		const Device* kDevice { Service::get<Device>() };

		if (mLayout)
			vkDestroyDescriptorSetLayout(kDevice->device(), mLayout, nullptr);
	}

	MaterialLayout::Layout& MaterialLayout::Layout::operator=(Layout&& other)
	{
		const Device* kDevice { Service::get<Device>() };

		if (mLayout && mLayout != other.mLayout)
			vkDestroyDescriptorSetLayout(kDevice->device(), mLayout, nullptr);

		mLayout = other.mLayout;

		other.mLayout = VK_NULL_HANDLE;

		return *this;
	}

	MaterialLayout::MaterialLayout(JsonNode* root)
	{
		load(root);
		createDescriptorPool(mMaxInstanceCount);
		initialize();
	}

	MaterialLayout::MaterialLayout(SetMap&& sets, uint32_t maxInstanceCount)
		: mMaxInstanceCount { maxInstanceCount }, mSets { std::move(sets) }, mPool { VK_NULL_HANDLE }, mSetLayouts {}
	{
		createDescriptorPool(mMaxInstanceCount == 0u ? skMaxMaterialInstance : mMaxInstanceCount);
		initialize();
	}

	MaterialLayout::MaterialLayout(const MaterialLayout& kOther, SetMap&& sets)
		: mMaxInstanceCount { kOther.mMaxInstanceCount }, mSets { kOther.mSets }, mPool { VK_NULL_HANDLE }, mSetLayouts {}
	{
		for (const std::pair<uint32_t, LayoutSet>& kPair : sets)
		{
			if (mSets.count(kPair.first) == 0u)
				mSets.emplace(kPair.first, kPair.second);
			else
				mSets.at(kPair.first) = kPair.second;
		}

		createDescriptorPool(mMaxInstanceCount == 0u ? skMaxMaterialInstance : mMaxInstanceCount);
		initialize();
	}

	MaterialLayout::~MaterialLayout()
	{
		Service::get<Renderer>()->wait();

		remove(this);

		const Device* kDevice { Service::get<Device>() };

		if (mPool)
			vkDestroyDescriptorPool(kDevice->device(), mPool, nullptr);
	}

	SAVE_FILE_CUSTOM_SIGNATURE(MaterialLayout)
	{
		SAVE_FILE_BODY()
	}

	SAVE_JSON_CUSTOM_SIGNATURE(MaterialLayout)
	{
		uint32_t previousMaxInstance { mMaxInstanceCount };
		if (previousMaxInstance == 0u && sMaxInstanceCounts.count(this))
			mMaxInstanceCount = sMaxInstanceCounts.at(this);

		SAVE_JSON_BODY(MaterialLayout)
		mMaxInstanceCount = previousMaxInstance;
	}

	LOAD_JSON_CUSTOM_SIGNATURE(MaterialLayout)
	{
		LOAD_JSON_BODY(MaterialLayout)
	}

	void MaterialLayout::initialize()
	{
		if (mOnChange.empty())
		{
			mOnChange += [this]() -> void
			{
				Service::get<Renderer>()->wait();

				const Device* kDevice{ Service::get<Device>() };

				if (mPool)
					vkDestroyDescriptorPool(kDevice->device(), mPool, nullptr);

				createDescriptorPool(mMaxInstanceCount == 0u ? skMaxMaterialInstance : mMaxInstanceCount);
				initialize();
			};
		}

		uint32_t setCount { static_cast<uint32_t>(mSets.size()) };
		if (setCount == 0u)
			return;

		mSetLayouts.reserve(setCount);
		uint32_t maxSetIndex { mSets.rbegin()->first };
		for (uint32_t i { 0u } ; i <= maxSetIndex ; ++i)
		{
			if (mSets.count(i) != 0u)
				mSetLayouts.emplace_back(mSets[i].mBindings);
			else
				mSetLayouts.emplace_back();
		}
	}

	bool MaterialLayout::addReference() const
	{
		if (sInstanceCounts.count(this) == 0u)
		{
			sInstanceCounts.emplace(this, 0u);
			sMaxInstanceCounts.emplace(this, 0u);
		}

		uint32_t& count { sInstanceCounts.at(this) };
		uint32_t& maxCount { sMaxInstanceCounts.at(this) };

		if (mMaxInstanceCount != 0u)
		{
			if (count >= mMaxInstanceCount)
				return false;
		}
		else if (count >= skMaxMaterialInstance)
			return false;

		++count;
		if (count > maxCount)
			maxCount = count;

		return true;
	}

	void MaterialLayout::removeReference() const
	{
		if (sInstanceCounts.count(this) == 0u)
			return;

		uint32_t& count { sInstanceCounts.at(this) };
		if (count == 0u)
			return;

		--count;
	}

	const std::map<uint32_t, LayoutSet>& MaterialLayout::sets() const
	{
		return mSets;
	}

	const LayoutSet* MaterialLayout::set(uint32_t index) const
	{
		if (mSets.count(index) == 0u)
			return nullptr;

		return &mSets.at(index);
	}

	const LayoutBinding* MaterialLayout::binding(uint32_t setIndex, uint32_t bindingIndex) const
	{
		if (mSets.count(setIndex) == 0u)
			return nullptr;

		const LayoutSet& kSet { mSets.at(setIndex) };
		if (kSet.mBindings.count(bindingIndex) == 0u)
			return nullptr;

		return &kSet.mBindings.at(bindingIndex);
	}

	uint32_t MaterialLayout::bindingCount() const
	{
		uint32_t bindingCount { 0u };

		for (const std::pair<const uint32_t, LayoutSet>& kPair : mSets)
			bindingCount += static_cast<uint32_t>(kPair.second.mBindings.size());

		return bindingCount;
	}

	VkDescriptorPool MaterialLayout::pool() const
	{
		return mPool;
	}

	const std::vector<MaterialLayout::Layout>& MaterialLayout::pipelineLayouts() const
	{
		return mSetLayouts;
	}

	std::vector<VkDescriptorSetLayout> MaterialLayout::descriptorLayouts() const
	{
		uint32_t setCount { static_cast<uint32_t>(mSets.size()) };
		if (setCount == 0u)
			return {};

		uint32_t layoutCount { static_cast<uint32_t>(mSetLayouts.size()) };
		
		std::vector<VkDescriptorSetLayout> layouts {};
		layouts.reserve(setCount);

		for (uint32_t i { 0u } ; i < layoutCount ; ++i)
		{
			if (mSets.count(i) != 0u)
				layouts.emplace_back(mSetLayouts[i].mLayout);
		}

		return layouts;
	}

	std::vector<VkDescriptorPoolSize> MaterialLayout::descriptorPoolSizes() const
	{
		std::unordered_map<EDescriptorType, uint32_t> countByType {};

		for (const std::pair<const uint32_t, LayoutSet>& kSetPair : mSets)
		{
			for (const std::pair<const uint32_t, LayoutBinding>& kBindPair : kSetPair.second.mBindings)
			{
				if (kBindPair.second.mCount == 0u)
					continue;

				countByType[kBindPair.second.mType] += kBindPair.second.mCount;
			}
		}

		std::vector<VkDescriptorPoolSize> poolSizes {};
		poolSizes.reserve(countByType.size());

		for (const std::pair<EDescriptorType, uint32_t>& kCount : countByType)
			poolSizes.emplace_back(VkDescriptorPoolSize { toDescriptorType(kCount.first), kCount.second });

		return poolSizes;
	}

	void MaterialLayout::createDescriptorPool(uint32_t maxInstanceCount)
	{
		std::vector<VkDescriptorPoolSize> poolSizes { descriptorPoolSizes() };

		uint32_t poolSizeCount { static_cast<uint32_t>(poolSizes.size()) };
		for (uint32_t i { 0u } ; i < poolSizeCount ; ++i)
			poolSizes[i].descriptorCount *= maxInstanceCount;

		VkDescriptorPoolCreateInfo poolInfo { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
		poolInfo.poolSizeCount = poolSizeCount;
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolInfo.maxSets = static_cast<uint32_t>(mSets.size()) * maxInstanceCount;

		VkResult result { vkCreateDescriptorPool(Service::get<Device>()->device(), &poolInfo, nullptr, &mPool) };
		if (result != VK_SUCCESS)
			THROW("failed to create descriptor pool with error: " + std::to_string(result))
	}
}