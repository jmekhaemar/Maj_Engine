#ifndef _LOGGING_H_
#define _LOGGING_H_

#include "FileOutput.h"
#include "StdOutput.h"
#include "OutputLevelRunTimeSwitch.h"
#include "NullOutput.h"

#include "Logger.h"

#ifndef _DEBUG
	#define LOGGING_DISABLE
#endif

#ifdef LOGGING_DISABLE

	LOGGING_DEFINE_OUTPUT(maj::log::FileOutput)

	#undef LOGGING_DEFINE_OUTPUT
	#define LOGGING_DEFINE_OUTPUT(NAME)

#else

	#ifndef LOG_CUSTOM_OUTPUT
		LOGGING_DEFINE_OUTPUT(maj::log::OutputLevelRunTimeSwitch<maj::log::StdOutput>)
	#endif

#endif

#endif