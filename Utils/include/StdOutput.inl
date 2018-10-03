#ifndef _STD_OUTPUT_INL_
#define _STD_OUTPUT_INL_

#include "StdOutput.h"

namespace maj::log
{
	template<typename T>
	void StdOutput::print(const T& kMessage)
	{
		std::cout << kMessage;
	}

	template<typename T>
	StdOutput& operator<<(StdOutput& out, const T& kMessage)
	{
		out.print(kMessage);
		return out;
	}
}

#endif
