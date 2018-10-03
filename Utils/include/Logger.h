#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "LoggerLevel.h"

namespace maj::log
{
	struct LoggingOutput;

	template<typename Level = LogVoid>
	class Logger
	{
	public:
		typedef LoggingOutput ReturnType;

		static constexpr ReturnType& transmit();
		static constexpr ReturnType& logging();

		static const char skEndl;
		static const char skTab;

	private:
		static constexpr ReturnType& obj();
	};

	template<typename T>
	LoggingOutput& operator<<(LoggingOutput& log, const T& kMessage);
}

#define LOGGING_DEFINE_OUTPUT(BASE)			\
namespace maj::log							\
{											\
	struct LoggingOutput : public BASE		\
	{										\
		typedef BASE Base;					\
	};										\
}

#define LOGGING_DISABLE_LEVEL(LEVEL)							\
namespace maj::log												\
{																\
	template<>													\
	class Logger<LEVEL>											\
	{															\
	public:														\
		typedef NullOutput ReturnType;							\
		static constexpr ReturnType& transmit()						\
		{														\
			return *reinterpret_cast<ReturnType*>(nullptr);		\
		}														\
	};															\
}

#define LOG_NEW_LINE		maj::log::Logger<>::transmit() << maj::log::Logger<>::skEndl;
#define LOG(LEVEL, TEXT)	maj::log::Logger<LEVEL>::transmit() << TEXT << maj::log::Logger<>::skEndl;

#define THROW(TEXT)			throw std::runtime_error(std::string(TEXT) + maj::log::Logger<>::skEndl + '(' + __FILE__ + ':' + std::to_string(__LINE__) + ')');

#include "Logger.inl"

#endif