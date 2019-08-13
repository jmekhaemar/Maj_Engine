#ifndef _STD_OUTPUT_H_
#define _STD_OUTPUT_H_

#include <iostream>

#include "NonCopyable.h"

namespace maj::log
{
	class StdOutput : public NonCopyable
	{
	public:
		template<typename T>
		void print(const T& kMessage);
	};

	template<typename T>
	StdOutput& operator<<(StdOutput& out, const T& kMessage);
}

#include "StdOutput.inl"

#endif