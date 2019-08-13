#include "InputAttachment.h"

#include "VkHelper.h"

namespace maj::rhi
{
	InputAttachment::InputAttachment(InputAttachment&& other)
		: mInfo { other.mInfo }
	{
		other.mInfo.imageView = VK_NULL_HANDLE;
		other.mInfo.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	}

	InputAttachment::InputAttachment(const Image& kImage)
		: mInfo { VK_NULL_HANDLE, kImage.view(), toImageLayout(kImage.info().mLayout) }
	{}

	VkWriteDescriptorSet InputAttachment::writeDescriptorSet(VkDescriptorSet set, uint32_t binding) const
	{
		VkWriteDescriptorSet writeSet { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
		writeSet.dstSet = set;
		writeSet.dstBinding = binding;
		writeSet.dstArrayElement = 0u;
		writeSet.descriptorCount = 1u;
		writeSet.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
		writeSet.pImageInfo = &mInfo;

		return writeSet;
	}

	InputAttachment& InputAttachment::operator=(InputAttachment&& other)
	{
		mInfo = other.mInfo;

		other.mInfo.imageView = VK_NULL_HANDLE;
		other.mInfo.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		return *this;
	}
}