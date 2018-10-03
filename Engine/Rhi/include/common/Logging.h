#ifndef _RHI_LOGGING_H_
#define _RHI_LOGGING_H_

#include <Logging.h>

LOGGING_CREATE_LEVEL(RHIInfo, maj::log::Severity::INFO, "[ INFO ][ RHI ] ")
LOGGING_CREATE_LEVEL(RHIDebug, maj::log::Severity::DEBUG, "[ DEBUG ][ RHI ] ")
LOGGING_CREATE_LEVEL(RHIWarning, maj::log::Severity::WARNING, "[ WARNING ][ RHI ] ")
LOGGING_CREATE_LEVEL(RHIError, maj::log::Severity::ERR, "[ ERROR ][ RHI ] ")

#endif