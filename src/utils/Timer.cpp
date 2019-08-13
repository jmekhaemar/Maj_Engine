#include "Timer.h"

#include "Logging.h"

namespace maj
{
	Timer::Timer()
		: mIsLoop { false }, mTimer { 0.f }, mFunc {}
	{
		start();
	}

	Timer::~Timer()
	{
		stop();
	}

	void Timer::start()
	{
		mStart = std::chrono::high_resolution_clock::now();
		mEnd = mStart;
	}

	void Timer::step()
	{
		stop();
		if (mFunc)
		{
			float elapsedTime { duration<std::ratio<1, 1>>() };
			while (mTimer < elapsedTime)
			{
				try
				{
					mFunc();
				}
				catch (const std::exception& e)
				{
					LOG(LogInfo, "failed to step timer: " << e.what())
				}

				elapsedTime -= mTimer;
				if (!mIsLoop)
				{
					mFunc = nullptr;
					return;
				}
				else if (mTimer >= elapsedTime)
					start();
			}
		}
	}

	void Timer::stop()
	{
		mEnd = std::chrono::high_resolution_clock::now();
	}

	void Timer::isLoop(bool looping)
	{
		mIsLoop = looping;
	}
}