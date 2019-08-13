#ifndef _INPUT_ATTACHMENT_H_
#define _INPUT_ATTACHMENT_H_

#include "IDescriptor.h"
#include "Image.h"

namespace maj::rhi
{
	class InputAttachment final : public IDescriptor
	{
	public:
		InputAttachment(const Image& kImage);
		InputAttachment(InputAttachment&& other);
		~InputAttachment() = default;

		virtual VkWriteDescriptorSet writeDescriptorSet(VkDescriptorSet set, uint32_t binding) const override;

		InputAttachment& operator=(InputAttachment&& other);

	private:
		VkDescriptorImageInfo mInfo;
	};
}

#endif