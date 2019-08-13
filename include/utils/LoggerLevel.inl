#ifndef _LOGGER_LEVEL_INL_
#define _LOGGER_LEVEL_INL_

#include "LoggerLevel.h"

namespace maj::log
{
	template<typename M>
	constexpr const char* Severity::mask()
	{
		return M::mask();
	}
}

#endif
