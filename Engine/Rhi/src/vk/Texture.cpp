#include "Texture.h"

#include "VkHelper.h"
#include "Instance.h"
#include "Device.h"

namespace maj::rhi
{
	DEFINE_HASH_OVERLOAD(Texture)

	Texture::Texture(const Image* kImage, bool isSerialized)
		: mImage { kImage, isSerialized }, mSampler { VK_NULL_HANDLE }, mInfo {}
	{
		initialize();
	}

	Texture::Texture(Texture&& other)
		: mImage { std::move(other.mImage) }, mSampler { other.mSampler }, mInfo { std::move(other.mInfo) }
	{
		other.mSampler = VK_NULL_HANDLE;
	}

	Texture::~Texture()
	{
		const Device* kDevice { Service::get<Device>() };

		if (mSampler)
			vkDestroySampler(kDevice->device(), mSampler, nullptr);
	}

	void Texture::initialize()
	{
		if (mOnChange.empty())
		{
			mImage.onChange([this]() -> void { mOnChange(); });

			mOnChange += [this]() -> void
			{
				const Device* kDevice{ Service::get<Device>() };

				if (mSampler)
					vkDestroySampler(kDevice->device(), mSampler, nullptr);

				initialize();
			};
		}

		createSampler();
		createDescriptorInfo();
	}

	const Image* Texture::image() const
	{
		return mImage.get();
	}

	VkSampler Texture::sampler() const
	{
		return mSampler;
	}

	VkWriteDescriptorSet Texture::writeDescriptorSet(VkDescriptorSet set, uint32_t binding) const
	{
		VkWriteDescriptorSet writeSet { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
		writeSet.dstSet = set;
		writeSet.dstBinding = binding;
		writeSet.dstArrayElement = 0u;
		writeSet.descriptorCount = 1u;
		writeSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		writeSet.pImageInfo = &mInfo;

		return writeSet;
	}

	void Texture::createSampler()
	{
		VkSamplerCreateInfo samplerInfo { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = samplerInfo.addressModeU;
		samplerInfo.addressModeW = samplerInfo.addressModeU;

		const ExtensionList& kExtensions { Service::get<Instance>()->extensions() };
		samplerInfo.anisotropyEnable = kExtensions.mSamplerAnisotropy != EExtensionLevel::DISABLED;
		samplerInfo.maxAnisotropy = 16.f;

		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = false;
		samplerInfo.compareEnable = false;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.f;
		samplerInfo.minLod = 0.f;
		samplerInfo.maxLod = 0.f;

		VkResult result = vkCreateSampler(Service::get<Device>()->device(), &samplerInfo, nullptr, &mSampler);
		if (result != VK_SUCCESS)
			THROW("failed to create texture sampler with error: " + std::to_string(result))
	}

	void Texture::createDescriptorInfo()
	{
		mInfo = { mSampler, mImage->view(), toImageLayout(mImage->info().mLayout) };
	}
}