#ifndef _FILE_OUTPUT_H_
#define _FILE_OUTPUT_H_

#include <iostream>
#include <fstream>

#include "NonCopyable.h"

namespace maj::log
{
	class FileOutput : public NonCopyable
	{
	public:
		FileOutput();
		~FileOutput();

		template<typename T>
		void print(const T& kMessage);

	private:
		std::filebuf mFileBuffer;
		std::ostream mOutputStream;
	};

	template<typename T>
	FileOutput& operator<<(FileOutput& out, const T& kMessage);
}

#include "FileOutput.inl"

#endif