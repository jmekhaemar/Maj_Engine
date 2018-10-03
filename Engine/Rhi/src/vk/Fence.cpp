#include "Fence.h"

#include "common/Logging.h"
#include "Device.h"

namespace maj::rhi
{
	Fence::Fence()
		: mFence { VK_NULL_HANDLE }, mIsValid { false }
	{
		const Device* kDevice { Service::get<Device>() };
		VkFenceCreateInfo fenceInfo { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };

		VkResult result { vkCreateFence(kDevice->device(), &fenceInfo, nullptr, &mFence) };
		if (result != VK_SUCCESS)
			THROW("failed to create semaphore with error: " + std::to_string(result))
	}

	Fence::Fence(Fence&& other)
		: mFence { other.mFence }, mIsValid { other.mIsValid }
	{
		other.mFence = VK_NULL_HANDLE;
	}

	Fence::~Fence()
	{
		const Device* kDevice { Service::get<Device>() };

		if (mFence)
			vkDestroyFence(kDevice->device(), mFence, nullptr);
	}

	void Fence::wait(uint64_t timeOut) const
	{
		if (!mIsValid || timeOut == 0u)
			return;

		const Device* kDevice { Service::get<Device>() };
		VkResult result { vkWaitForFences(kDevice->device(), 1u, &mFence, true, timeOut * 1000000u) };
		if (result != VK_SUCCESS)
			THROW("failed to wait for fence with error: " + std::to_string(result))
	}

	VkFence Fence::fence() const
	{
		return mFence;
	}

	void Fence::reset()
	{
		const Device* kDevice { Service::get<Device>() };

		VkResult result { vkResetFences(kDevice->device(), 1u, &mFence) };
		if (result != VK_SUCCESS)
			THROW("failed to reset fence with error: " + std::to_string(result))

		mIsValid = false;
	}

	void Fence::validate()
	{
		mIsValid = true;
	}

	Fence& Fence::operator=(Fence&& other)
	{
		const Device* kDevice { Service::get<Device>() };
		if (mFence)
			vkDestroyFence(kDevice->device(), mFence, nullptr);

		mFence = other.mFence;
		other.mFence = VK_NULL_HANDLE;

		mIsValid = other.mIsValid;
		return *this;
	}
}