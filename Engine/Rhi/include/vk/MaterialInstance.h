#ifndef _MATERIAL_INSTANCE_H_
#define _MATERIAL_INSTANCE_H_

#include <vulkan/vulkan.h>
#include <map>

#include <NonCopyable.h>

#include "Material.h"

namespace maj::rhi
{
	class MaterialInstance final : public NonCopyable, public ISerializable
	{
		ENABLE_META(MaterialInstance)

	private:
		struct Set final : public NonCopyable
		{
			Set(VkDescriptorSet set);
			Set(Set&& other);
			~Set() = default;

			Set& operator=(Set&& other);

			VkDescriptorSet mSet;
		};

	public:
		LOAD_CONSTRUCTOR(MaterialInstance)

		MaterialInstance(const Material* kMaterial);
		MaterialInstance(const Material* kMaterial, std::map<MaterialBinding, const IDescriptor*>&& kDescriptors);
		MaterialInstance(const MaterialInstance& kOther, const Material* kMaterial);
		MaterialInstance(MaterialInstance&& other);
		virtual ~MaterialInstance();

		void bind(VkCommandBuffer commandBuffer, bool hasChangedMaterial) const;

		const Material& material() const;

		const std::map<MaterialBinding, const IDescriptor*>& descriptors() const;
		void descriptor(uint32_t setIndex, uint32_t bindingIndex, const IDescriptor* kDescriptor);

		void update(uint32_t setIndex, uint32_t bindingIndex) const;
		void update(uint32_t setIndex) const;
		void update() const;

		MaterialInstance& operator=(MaterialInstance&& other);

	private:
		void createDescriptorSets();

		Asset<Material> mMaterial;
		const MaterialLayout* mMaterialLayout;
		std::map<MaterialBinding, const IDescriptor*> mDescriptors;

		std::map<uint32_t, Set> mSets;
	};
}

META_CLASS(maj::rhi::MaterialInstance, 1u)
META_FIELD(maj::rhi::MaterialInstance, maj::Asset<maj::rhi::Material>, mMaterial, 0u)

#endif