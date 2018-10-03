#ifndef _NULL_OUTPUT_H_
#define _NULL_OUTPUT_H_

#include "NonCopyable.h"

namespace maj::log
{
	class NullOutput : public NonCopyable
	{};

	template<typename T>
	NullOutput& operator<<(NullOutput& no, const T& kMessage);
}

#include "NullOutput.inl"

#endif