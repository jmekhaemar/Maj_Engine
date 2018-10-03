#ifndef _IDESCRIPTOR_H_
#define _IDESCRIPTOR_H_

#include <vulkan/vulkan.h>

#include <NonCopyable.h>

namespace maj::rhi
{
	class IDescriptor : public NonCopyable
	{
	public:
		virtual ~IDescriptor() = default;

		virtual VkWriteDescriptorSet writeDescriptorSet(VkDescriptorSet set, uint32_t binding) const = 0;
	};
}

#endif