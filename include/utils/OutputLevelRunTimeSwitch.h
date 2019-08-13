#ifndef _OUTPUT_LEVEL_RUN_TIME_SWITCH_H_
#define _OUTPUT_LEVEL_RUN_TIME_SWITCH_H_

#include "LoggerLevel.h"

namespace maj::log
{
	template<typename Output>
	class OutputLevelRunTimeSwitch
	{
	public:
		bool isAllowed();
		void changeCurrentSeverity(Severity::ESeverities severity);

		Output mOutput;

	private:
		Severity mSeverity;
		Severity mCurrent;
	};

	template<typename Output, typename M>
	OutputLevelRunTimeSwitch<Output>& operator<<(OutputLevelRunTimeSwitch<Output>& out, const M& kMessage);

	template<typename Output>
	OutputLevelRunTimeSwitch<Output>& operator<<(OutputLevelRunTimeSwitch<Output>& out, Severity::ESeverities severity);
}

#include "OutputLevelRunTimeSwitch.inl"

#endif