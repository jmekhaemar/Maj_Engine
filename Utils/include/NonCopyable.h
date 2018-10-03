#ifndef _NON_COPYABLE_H_
#define _NON_COPYABLE_H_

namespace maj
{
	class NonCopyable
	{
	protected:
		NonCopyable() = default;
		~NonCopyable() = default;

		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;
	};
}

#endif