#ifndef _FILE_OUTPUT_INL_
#define _FILE_OUTPUT_INL_

#include "FileOutput.h"

namespace maj::log
{
	template<typename T>
	void FileOutput::print(const T& kMessage)
	{
		mOutputStream << kMessage;
	}

	template<typename T>
	FileOutput& operator<<(FileOutput& out, const T& kMessage)
	{
		out.print(kMessage);
		return out;
	}
}

#endif