#include "FileOutput.h"

namespace maj::log
{
	FileOutput::FileOutput()
		: mOutputStream{ &mFileBuffer }
	{
		mFileBuffer.open("log", std::ios::out);
	}

	FileOutput::~FileOutput()
	{
		mFileBuffer.close();
	}
}