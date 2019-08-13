#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <queue>
#include <atomic>

#include "Service.h"

namespace maj
{
	class ThreadPool final : public Service
	{
	public:
		ThreadPool();
		~ThreadPool();

		template<typename Func, typename... Args>
		std::future<void> submit(Func&& func, Args&&... args);

	private:
		void loop();

		std::atomic_bool mIsRun;
		std::mutex mThreadMutex;
		std::mutex mFunctionsMutex;

		std::condition_variable mIsFunctionAvailable;
		std::vector<std::thread> mThreads;
		std::queue<std::packaged_task<void()>> mFunctions;
	};
}

#include "ThreadPool.inl"

#endif