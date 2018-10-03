#ifndef _TEST_H_
#define _TEST_H_

#include <iomanip>
#include <limits>

#undef _DEBUG
#define _DEBUG
#define LOG_CUSTOM_OUTPUT
#include <Logging.h>

LOGGING_DEFINE_OUTPUT(maj::log::OutputLevelRunTimeSwitch<maj::log::FileOutput>)

#define ASSERT_FAILED LOG(LogError, "Failed assertion (" << __FILE__ << ':' << std::to_string(__LINE__) << ')')

#define TEST_CLASS(NAME)						\
int main()										\

#define TEST_METHOD(NAME)						\
LOG_NEW_LINE									\
LOG(LogInfo, std::string("test_") + NAME)		\

#define AreEqual(LHS, RHS, PRECISION)															\
LOG(LogVoid, "Assert are equal " << std::to_string(LHS) << " | " << std::to_string(RHS))		\
if (std::fabs((LHS) - (RHS)) > PRECISION)														\
{																								\
	ASSERT_FAILED																				\
	exit(EXIT_FAILURE);																			\
}																								\

#define AreNotEqual(LHS, RHS, PRECISION)														\
LOG(LogVoid, "Assert are not equal " << std::to_string(LHS) << " | " << std::to_string(RHS))	\
if (std::fabs((LHS) - (RHS)) < PRECISION)														\
{																								\
	ASSERT_FAILED																				\
	exit(EXIT_FAILURE);																			\
}																								\

#define IsTrue(CONDITION)										\
LOG(LogVoid, "Assert is true " << std::to_string(CONDITION))	\
if (!(CONDITION))												\
{																\
	ASSERT_FAILED												\
	exit(EXIT_FAILURE);											\
}																\

#define IsFalse(CONDITION)										\
LOG(LogVoid, "Assert is false " << std::to_string(CONDITION))	\
if (CONDITION)													\
{																\
	ASSERT_FAILED												\
	exit(EXIT_FAILURE);											\
}																\

#endif