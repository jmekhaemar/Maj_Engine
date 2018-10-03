#ifndef _THREAD_POOL_INL_
#define _THREAD_POOL_INL_

#include "ThreadPool.h"

namespace maj
{
	template<typename Func, typename... Args>
	std::future<void> ThreadPool::submit(Func&& func, Args&&... args)
	{
		std::unique_lock<std::mutex> lock(mFunctionsMutex);

		mFunctions.emplace(std::bind(func, std::forward<Args>(args)...));
		mIsFunctionAvailable.notify_one();

		return mFunctions.back().get_future();
	}
}

#endif