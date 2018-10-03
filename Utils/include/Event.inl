#ifndef _EVENT_INL_
#define _EVENT_INL_

#include "Event.h"

namespace maj
{
	template<typename R, typename... Args>
	Event<R, Args...>::Event()
		: mCounter{ 0u }, mFunctions{}
	{}

	template <typename R, typename... Args>
	bool Event<R, Args...>::empty() const
	{
		return mCounter == 0u;
	}

	template<typename R, typename... Args>
	void Event<R, Args...>::clear()
	{
		mCounter = 0u;
		mFunctions.clear();
	}

	template<typename R, typename... Args>
	uint32_t Event<R, Args...>::operator+(const std::function<R(Args...)>& kFunc)
	{
		mFunctions.emplace(std::make_pair(++mCounter, kFunc));
		return mCounter;
	}

	template<typename R, typename... Args>
	Event<R, Args...>& Event<R, Args...>::operator+=(const std::function<R(Args...)>& kFunc)
	{
		mFunctions.emplace(std::make_pair(++mCounter, kFunc));
		return *this;
	}

	template<typename R, typename... Args>
	Event<R, Args...>& Event<R, Args...>::operator-=(uint32_t key)
	{
		mFunctions.erase(key);
		return *this;
	}

	template<typename R, typename... Args>
	void Event<R, Args...>::operator()(Args&&... args) const
	{
		for (const std::pair<uint32_t, std::function<R(Args...)>>& kPair : mFunctions)
			kPair.second(std::forward<Args>(args)...);
	}
}

#endif