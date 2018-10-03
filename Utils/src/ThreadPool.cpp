#include "ThreadPool.h"

#include "Logging.h"

namespace maj
{
	ThreadPool::ThreadPool()
		: Service{ typeid(ThreadPool).name() }, mIsRun{ true }, mThreads{ std::thread::hardware_concurrency() }
	{
		for (uint8_t i{ 0u }; i < mThreads.size() ; ++i)
			mThreads[i] = std::thread{ &ThreadPool::loop, this };
	}

	ThreadPool::~ThreadPool()
	{
		mIsRun = false;
		mIsFunctionAvailable.notify_all();
		for (uint8_t i{ 0u }; i < mThreads.size(); ++i)
			if(mThreads[i].joinable())
				mThreads[i].join();
			else
				LOG(LogError, "failed to destroy thread pool: thread " << i << " not joinable")
	}

	void ThreadPool::loop()
	{
		while (true)
		{
			std::packaged_task<void()> task;
			{
				{
					std::unique_lock<std::mutex> lock{ mThreadMutex };
					mIsFunctionAvailable.wait(lock, [this] { return !mFunctions.empty() || !mIsRun; });
				}

				std::unique_lock<std::mutex> lock{ mFunctionsMutex };
				if (mFunctions.empty())
					return;
				task = std::move(mFunctions.front());
				mFunctions.pop();
			}
			task();
		}
	}
}