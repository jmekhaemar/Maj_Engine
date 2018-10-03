#ifndef _LOGGER_LEVEL_H_
#define _LOGGER_LEVEL_H_

#include <bitset>

namespace maj::log
{
	struct SeveritiesMask;

	struct Severity final
	{
		enum ESeverities
		{
			ERR = 1u,
			WARNING,
			NORMAL,
			INFO,
			DEBUG,
			USER
		};

		template<typename M = SeveritiesMask>
		static constexpr const char* mask();

		Severity();

		bool operator& (Severity severity);
		Severity& operator= (ESeverities severity);

		ESeverities mSeverity;
		std::bitset<USER> mEnabledSeverities;
	};
}


#define LOGGING_CREATE_LEVEL(LEVELNAME, SEVERITY, DESC)	\
namespace maj::log										\
{														\
	struct LEVELNAME final								\
	{													\
		static Severity::ESeverities severity()			\
		{												\
			return SEVERITY;							\
		}												\
														\
		static const char* desc()						\
		{												\
			return DESC;								\
		}												\
	};													\
}														\
typedef maj::log::LEVELNAME Log ## LEVELNAME;

#define LOGGING_DEFINE_SEVERITIES_MASK(MASK)		\
namespace maj::log									\
{													\
	struct SeveritiesMask final						\
	{												\
		static const char* mask()					\
		{											\
			return MASK;							\
		}											\
	};												\
}

LOGGING_CREATE_LEVEL(Void,		maj::log::Severity::NORMAL,	"")
LOGGING_CREATE_LEVEL(Info,		maj::log::Severity::INFO,	"[ INFO ] ")
LOGGING_CREATE_LEVEL(Debug,		maj::log::Severity::DEBUG,	"[ DEBUG ] ")
LOGGING_CREATE_LEVEL(Warning,	maj::log::Severity::WARNING, "[ WARNING ] ")
LOGGING_CREATE_LEVEL(Error,		maj::log::Severity::ERR,	"[ ERROR ] ")

LOGGING_DEFINE_SEVERITIES_MASK("111111")


#include "LoggerLevel.inl"

#endif