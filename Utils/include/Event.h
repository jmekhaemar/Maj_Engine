#ifndef _EVENT_H_
#define _EVENT_H_

#include <unordered_map>
#include <functional>

namespace maj
{
	template<typename R, typename... Args>
	class Event final
	{
	public:
		Event();
		~Event() = default;

		bool empty() const;
		void clear();

		uint32_t operator+(const std::function<R(Args...)>& kFunc);
		Event& operator+=(const std::function<R(Args...)>& kFunc);
		Event& operator-=(uint32_t key);
		void operator()(Args&&... args) const;

	private:
		uint32_t mCounter;
		std::unordered_map<uint32_t, std::function<R(Args...)>> mFunctions;
	};
}

#include "Event.inl"

#endif