#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "IDescriptor.h"
#include "Image.h"

namespace maj::rhi
{
	class Texture final : public IDescriptor, public ISerializable
	{
		ENABLE_META(Texture)

	public:
		LOAD_CONSTRUCTOR(Texture)

		Texture(const Image* kImage, bool isSerialized = true);
		Texture(Texture&& other);
		virtual ~Texture();

		const Image* image() const;
		VkSampler sampler() const;

		virtual VkWriteDescriptorSet writeDescriptorSet(VkDescriptorSet set, uint32_t binding) const override;

		Event<void> mOnChange;

	private:
		void createSampler();
		void createDescriptorInfo();

		Asset<Image> mImage;

		VkSampler mSampler;
		VkDescriptorImageInfo mInfo;
	};
}

META_CLASS(maj::Asset<maj::rhi::Texture>, 1u)
META_FIELD(maj::Asset<maj::rhi::Texture>, uint32_t, mGuid, 0u)

META_CLASS(maj::rhi::Texture, 2u)
META_FIELD(maj::rhi::Texture, uint32_t, mGuid, 0u)
META_FIELD(maj::rhi::Texture, maj::Asset<maj::rhi::Image>, mImage, 1u)

#endif