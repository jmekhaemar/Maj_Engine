#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>
#include <chrono>

#include "NonCopyable.h"

namespace maj
{
	class Timer final : public NonCopyable
	{
	public:
		Timer();
		template<typename Func, typename... Args>
		Timer(float timer, Func&& func, Args&&... args);
		~Timer();

		template<typename Func, typename... Args>
		void set(float timer, Func&& func, Args&&... args);

		void start();
		void step();
		void stop();

		void isLoop(bool looping);

		template<typename T>
		float duration() const;

	private:
		std::chrono::steady_clock::time_point mStart;
		std::chrono::steady_clock::time_point mEnd;

		bool mIsLoop;
		float mTimer;
		std::function<void()> mFunc;
	};
}

#include "Timer.inl"

#endif