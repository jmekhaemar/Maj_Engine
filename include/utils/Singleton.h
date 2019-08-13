#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include "NonCopyable.h"

namespace maj
{
	template<typename T>
	class Singleton : public NonCopyable
	{
	public:
		constexpr static T& instance();

	protected:
		static T sObj;

	public:
		virtual ~Singleton() = default;

	protected:
		Singleton() = default;
	};
}

#include "Singleton.inl"

#endif