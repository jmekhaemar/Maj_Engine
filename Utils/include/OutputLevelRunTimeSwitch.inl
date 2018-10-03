#ifndef _OUTPUT_LEVEL_RUN_TIME_SWITCH_INL_
#define _OUTPUT_LEVEL_RUN_TIME_SWITCH_INL_

#include "OutputLevelRunTimeSwitch.h"

namespace maj::log
{
	template<typename Output>
	bool OutputLevelRunTimeSwitch<Output>::isAllowed()
	{
		return mSeverity & mCurrent;
	}

	template<typename Output>
	void OutputLevelRunTimeSwitch<Output>::changeCurrentSeverity(Severity::ESeverities severity)
	{
		mCurrent = severity;
	}

	template<typename Output, typename M>
	OutputLevelRunTimeSwitch<Output>& operator<< (OutputLevelRunTimeSwitch<Output>& out, const M& kMessage)
	{
		if (out.isAllowed())
			out.mOutput << kMessage;

		return out;
	}

	template<typename Output>
	OutputLevelRunTimeSwitch<Output>& operator<< (OutputLevelRunTimeSwitch<Output>& out, Severity::ESeverities severity)
	{
		out.changeCurrentSeverity(severity);
		return out;
	}
}

#endif

