#ifndef _FENCE_H_
#define _FENCE_H_

#include <vulkan/vulkan.h>

#include <NonCopyable.h>

namespace maj::rhi
{
	class Fence final : public NonCopyable
	{
	public:
		Fence();
		Fence(Fence&& other);
		~Fence();

		// Time out in ms
		void wait(uint64_t timeOut) const;
		VkFence fence() const;

		void reset();
		void validate();

		Fence& operator=(Fence&& other);

	private:
		VkFence mFence;
		bool mIsValid;
	};

}

#endif