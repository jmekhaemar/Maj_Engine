#include "MaterialInstance.h"

#include <Window.h>

#include "common/Logging.h"

#include "Renderer.h"

namespace maj::rhi
{
	DEFINE_HASH_OVERLOAD(MaterialInstance)

	MaterialInstance::Set::Set(VkDescriptorSet set)
		: mSet { set }
	{}

	MaterialInstance::Set::Set(Set&& other)
		: mSet { other.mSet }
	{
		other.mSet = VK_NULL_HANDLE;
	}

	MaterialInstance::Set& MaterialInstance::Set::operator=(Set&& other)
	{
		mSet = other.mSet;
		other.mSet = VK_NULL_HANDLE;

		return *this;
	}

	MaterialInstance::MaterialInstance(const Material* kMaterial)
		: mMaterial { kMaterial }, mDescriptors {}, mSets {}
	{
		initialize();
	}

	MaterialInstance::MaterialInstance(const Material* kMaterial, std::map<MaterialBinding, const IDescriptor*>&& kDescriptors)
		: mMaterial { kMaterial }, mDescriptors { std::move(kDescriptors) }, mSets {}
	{
		initialize();
	}

	MaterialInstance::MaterialInstance(const MaterialInstance& kOther, const Material* kMaterial)
		: mMaterial { kMaterial, false }, mDescriptors { kOther.mDescriptors }, mSets {}
	{
		initialize();
	}

	MaterialInstance::MaterialInstance(MaterialInstance&& other)
		: mMaterial { other.mMaterial }, mDescriptors { std::move(other.mDescriptors) } , mSets { std::move(other.mSets) }
	{}

	MaterialInstance::~MaterialInstance()
	{
		Renderer* renderer { Service::get<Renderer>() };
		renderer->wait();

		const Device* kDevice{ Service::get<Device>() };
		if (!mMaterial.empty())
		{
			renderer->frameGraph().remove(this);

			const MaterialLayout& kLayout { mMaterial->layout() };

			std::vector<VkDescriptorSet> sets {};
			uint32_t setCount { static_cast<uint32_t>(mSets.size()) };
			sets.reserve(setCount);
			for (const std::pair<const uint32_t, Set>& kPair : mSets)
				sets.emplace_back(kPair.second.mSet);

			vkFreeDescriptorSets(kDevice->device(), kLayout.pool(), setCount, sets.data());
			kLayout.removeReference();
		}
	}
	
	void MaterialInstance::initialize()
	{
		if (mMaterial.empty())
			THROW("failed to create material instance: no material")

		const MaterialLayout& kLayout { mMaterial->layout() };
		mMaterialLayout = &kLayout;
		if (!kLayout.addReference())
			THROW("failed to create material instance: max instance count for the layout exceeded")

		createDescriptorSets();

		mMaterial.onChange([this]() -> void 
		{ 
			Renderer* renderer{ Service::get<Renderer>() };
			if (mMaterialLayout != &mMaterial->layout())
			{
				renderer->wait();

				const Device* kDevice{ Service::get<Device>() };
				renderer->frameGraph().remove(this);

				std::vector<VkDescriptorSet> sets{};
				uint32_t setCount{ static_cast<uint32_t>(mSets.size()) };
				sets.reserve(setCount);
				for (const std::pair<const uint32_t, Set>& kPair : mSets)
					sets.emplace_back(kPair.second.mSet);

				vkFreeDescriptorSets(kDevice->device(), mMaterialLayout->pool(), setCount, sets.data());
				mMaterialLayout->removeReference();

				mMaterialLayout = &mMaterial->layout();
				if (!mMaterialLayout->addReference())
					THROW("failed to create material instance: max instance count for the layout exceeded")

				createDescriptorSets();
				renderer->frameGraph().dirty(*mMaterial);
			}
			else
				update(); 

			renderer->frameGraph().dirty(mMaterial->info().renderPass());
		});
	}

	void MaterialInstance::bind(VkCommandBuffer commandBuffer, bool hasChangedMaterial) const
	{
		// Bind material pipeline
		if (hasChangedMaterial)
		{
			mMaterial->bindPipeline(commandBuffer);

			// Set dynamic state (viewport/scissor)
			const std::unordered_map<uint8_t, ww::Viewport>& kViewports { Service::get<ww::Window>()->viewports() };
			std::vector<VkViewport> viewports {};
			std::vector<VkRect2D> scissors {};

			uint8_t count { static_cast<uint8_t>(kViewports.size()) };
			viewports.reserve(count);
			scissors.reserve(count);

			for (const std::pair<const uint8_t, ww::Viewport>& kPair : kViewports)
			{
				const math::Vec2ui& kExtent { kPair.second.extent() };

				viewports.emplace_back(VkViewport { 0.f, 0.f, static_cast<float>(kExtent.mX), static_cast<float>(kExtent.mY), 0.f, 1.f });
				scissors.emplace_back(VkRect2D { { 0, 0 }, { kExtent.mX, kExtent.mY } });
			}

			vkCmdSetViewport(commandBuffer, 0u, count, viewports.data());
			vkCmdSetScissor(commandBuffer, 0u, count, scissors.data());
		}

		// Bind descriptor sets
		std::map<uint32_t, std::vector<VkDescriptorSet>> sets {};
		uint32_t prevIndex { 0u };
		uint32_t crtFirstIndex { 0u };
		bool once { true };
		for (const std::pair<const uint32_t, Set>& kPair : mSets)
		{
			if (once)
			{
				sets.emplace(kPair.first, std::vector<VkDescriptorSet>{});
				crtFirstIndex = kPair.first;
				once = false;
			}
			else if (kPair.first - prevIndex > 1u)
			{
				sets.emplace(kPair.first, std::vector<VkDescriptorSet>{});
				crtFirstIndex = kPair.first;
			}

			sets[crtFirstIndex].emplace_back(kPair.second.mSet);
			prevIndex = kPair.first;
		}

		mMaterial->bindDescriptors(commandBuffer, sets);
	}

	const Material& MaterialInstance::material() const
	{
		return *mMaterial;
	}

	const std::map<MaterialBinding, const IDescriptor*>& MaterialInstance::descriptors() const
	{
		return mDescriptors;
	}

	void MaterialInstance::descriptor(uint32_t setIndex, uint32_t bindingIndex, const IDescriptor* kDescriptor)
	{
		// Check if this set/binding is used in the material
		const MaterialLayout& kLayout { mMaterial->layout() };
		const LayoutBinding* kBinding { kLayout.binding(setIndex, bindingIndex) };
		if (!kBinding)
		{
			LOG(LogRHIWarning, "failed to set descriptor: descriptor at " << setIndex << "." << bindingIndex << " not found")
			return;
		}

		// Set/assign the descriptor
		MaterialBinding coord { setIndex, bindingIndex };
		if (mDescriptors.count(coord) != 0u)
			mDescriptors[coord] = kDescriptor;
		else
			mDescriptors.emplace(coord, kDescriptor);
	}

	void MaterialInstance::update(uint32_t setIndex, uint32_t bindingIndex) const
	{
		if (mSets.count(setIndex) == 0u)
			return;

		const Set& kSet { mSets.at(setIndex) };
		MaterialBinding coord { setIndex, bindingIndex };

		// If no instanced descriptor, use default if binding is a texture
		const IDescriptor* kDescriptor { nullptr };
		if (mDescriptors.count(coord) != 0u)
			kDescriptor = mDescriptors.at(coord);

		else
		{
			// If no default descriptor, do nothing
			kDescriptor = mMaterial->texture(setIndex, bindingIndex);
			if (!kDescriptor)
			{
				LOG(LogRHIWarning, "failed to update descriptor: descriptor at " << setIndex << "." << bindingIndex << " not found")
				return;
			}
		}

		VkWriteDescriptorSet descriptorWrite { kDescriptor->writeDescriptorSet(kSet.mSet, bindingIndex) };

		rhi::Renderer* renderer{ Service::get<rhi::Renderer>() };
		renderer->wait();

		vkUpdateDescriptorSets(Service::get<Device>()->device(), 1u, &descriptorWrite, 0u, nullptr);

		renderer->frameGraph().dirty(*mMaterial);
	}

	void MaterialInstance::update(uint32_t setIndex) const
	{
		const LayoutSet* kSetLayout { mMaterial->layout().set(setIndex) };
		if (mSets.count(setIndex) == 0u && kSetLayout)
			return;

		const Set& kSet { mSets.at(setIndex) };		
		uint32_t bindingCount { static_cast<uint32_t>(kSetLayout->mBindings.size()) };

		std::vector<VkWriteDescriptorSet> descriptorWrites {};
		descriptorWrites.reserve(bindingCount);

		// For each binding of this set
		for (const std::pair<const uint32_t, LayoutBinding>& kPair : kSetLayout->mBindings)
		{
			MaterialBinding coord { setIndex, kPair.first };

			// If no instanced descriptor, use default if binding is a texture
			const IDescriptor* kDescriptor { nullptr };
			if (mDescriptors.count(coord) != 0u)
				kDescriptor = mDescriptors.at(coord);

			else
			{
				// If no default descriptor, do nothing
				kDescriptor = mMaterial->texture(setIndex, kPair.first);
				if (!kDescriptor)
				{
					LOG(LogRHIWarning, "failed to update descriptor set: descriptor at " << setIndex << "." << kPair.first << " not found")
					--bindingCount;
					continue;
				}
			}

			descriptorWrites.emplace_back(kDescriptor->writeDescriptorSet(kSet.mSet, kPair.first));
		}

		rhi::Renderer* renderer{ Service::get<rhi::Renderer>() };
		renderer->wait();

		vkUpdateDescriptorSets(Service::get<Device>()->device(), bindingCount, descriptorWrites.data(), 0u, nullptr);

		renderer->frameGraph().dirty(*mMaterial);
	}

	void MaterialInstance::update() const
	{
		const std::map<uint32_t, LayoutSet>& kSets { mMaterial->layout().sets() };

		std::vector<VkWriteDescriptorSet> descriptorWrites {};
		uint32_t descriptorCount { mMaterial->layout().bindingCount() };
		descriptorWrites.reserve(descriptorCount);
	
		// For each set
		for (const std::pair<const uint32_t, LayoutSet>& kSetPair : kSets)
		{
			const Set& kSet { mSets.at(kSetPair.first) };

			// For each binding
			for (const std::pair<const uint32_t, LayoutBinding>& kBindPair : kSetPair.second.mBindings)
			{
				MaterialBinding coord { kSetPair.first, kBindPair.first };

				// If no instanced descriptor, use default if binding is a texture
				const IDescriptor* kDescriptor { nullptr };
				if (mDescriptors.count(coord) != 0u)
					kDescriptor = mDescriptors.at(coord);

				else
				{
					// If no default descriptor, do nothing
					kDescriptor = mMaterial->texture(kSetPair.first, kBindPair.first);
					if (!kDescriptor)
					{
						LOG(LogRHIWarning, "failed to update descriptors: descriptor at " << kSetPair.first << "." << kBindPair.first << " not found")
						--descriptorCount;
						continue;
					}
				}

				descriptorWrites.emplace_back(kDescriptor->writeDescriptorSet(kSet.mSet, kBindPair.first));
			}
		}

		rhi::Renderer* renderer{ Service::get<rhi::Renderer>() };
		renderer->wait();

		vkUpdateDescriptorSets(Service::get<Device>()->device(), descriptorCount, descriptorWrites.data(), 0u, nullptr);

		renderer->frameGraph().dirty(*mMaterial);
	}

	MaterialInstance& MaterialInstance::operator=(MaterialInstance&& other)
	{
		Service::get<rhi::Renderer>()->wait();

		const Device* kDevice { Service::get<Device>() };
		if (!mMaterial.empty())
		{
			const MaterialLayout& klayout { mMaterial->layout() };

			std::vector<VkDescriptorSet> sets {};
			uint32_t setCount { static_cast<uint32_t>(mSets.size()) };
			sets.reserve(setCount);
			for (const std::pair<const uint32_t, Set>& kPair : mSets)
				sets.emplace_back(kPair.second.mSet);

			vkFreeDescriptorSets(kDevice->device(), klayout.pool(), setCount, sets.data());
			klayout.removeReference();
		}

		mMaterial = other.mMaterial;
		mDescriptors = std::move(other.mDescriptors);
		mSets = std::move(other.mSets);

		return *this;
	}

	void MaterialInstance::createDescriptorSets()
	{
		const MaterialLayout& kLayout { mMaterial->layout() };

		std::vector<VkDescriptorSetLayout> layouts { kLayout.descriptorLayouts() };
		uint32_t layoutCount { static_cast<uint32_t>(layouts.size()) };

		std::vector<VkDescriptorSet> sets {};
		sets.resize(layoutCount, VK_NULL_HANDLE);

		VkDescriptorSetAllocateInfo allocInfo { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
		allocInfo.descriptorPool = kLayout.pool();
		allocInfo.descriptorSetCount = layoutCount;
		allocInfo.pSetLayouts = layouts.data();

		VkResult result { vkAllocateDescriptorSets(Service::get<Device>()->device(), &allocInfo, sets.data()) };
		if (result != VK_SUCCESS)
			THROW("failed to allocate descriptor set with error: " + std::to_string(result))

		const std::map<uint32_t, LayoutSet>& kSets { kLayout.sets() };
		uint32_t i { 0u };
		for (const std::pair<const uint32_t, LayoutSet>& kPair : kSets)
		{
			mSets.emplace(kPair.first, sets[i]);
			++i;
		}
	}
}