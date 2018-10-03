#ifndef _NULL_OUTPUT_INL_
#define _NULL_OUTPUT_INL_

#include "NullOutput.h"

namespace maj::log
{
	template<typename T>
	NullOutput& operator<<(NullOutput& no, const T& kMessage)
	{
		(void) kMessage;
		return no;
	}
}

#endif
