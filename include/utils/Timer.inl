#ifndef _TIMER_INL_
#define _TIMER_INL_

#include "Timer.h"

namespace maj
{
	template<typename Func, typename... Args>
	Timer::Timer(float timer, Func&& func, Args&&... args)
		: mTimer { timer }, mFunc { std::bind(func, std::forward<Args>(args)...) }
	{
		start();
	}

	template<typename Func, typename... Args>
	void Timer::set(float timer, Func&& func, Args&&... args)
	{
		mTimer = timer;
		mFunc = std::bind(func, std::forward<Args>(args)...);
	}

	template<typename T>
	float Timer::duration() const
	{
		return std::chrono::duration<float, T>(mEnd - mStart).count();
	}
}

#endif