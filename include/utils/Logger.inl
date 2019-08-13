#ifndef _LOGGER_INL_
#define _LOGGER_INL_

#include "Logger.h"

#include "Singleton.h"

namespace maj::log
{
	template<>
	const char Logger<>::skEndl { '\n' };

	template<>
	const char Logger<>::skTab { '\t' };

	template<typename Level>
	constexpr typename Logger<Level>::ReturnType& Logger<Level>::obj()
	{
		return Singleton<Logger<Level>::ReturnType>::instance();
	}

	template<typename Level>
	constexpr typename Logger<Level>::ReturnType& Logger<Level>::logging()
	{
		return Logger<Level>::obj();
	}

	template<typename Level>
	constexpr typename Logger<Level>::ReturnType& Logger<Level>::transmit()
	{
		return Logger<Level>::logging() << Level::severity() << Level::desc();
	}

	template<>
	constexpr Logger<>::ReturnType& Logger<>::transmit()
	{
		return Logger<>::logging();
	}

	template<typename T>
	LoggingOutput& operator<< (LoggingOutput& log, const T& kMessage)
	{
		static_cast<typename LoggingOutput::Base&>(log) << kMessage;
		return log;
	}
}

#endif