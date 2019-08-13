#include "LoggerLevel.h"

namespace maj::log
{
	Severity::Severity() : mSeverity { USER }, mEnabledSeverities { std::bitset<USER>(mask<>()) }
	{ }

	bool Severity::operator& (Severity severity) {
		return mEnabledSeverities.test(severity.mSeverity - 1);
	}

	Severity& Severity::operator= (ESeverities severity) {
		mSeverity = severity;
		mEnabledSeverities = std::bitset<USER>(std::string(mask<>()), static_cast<size_t>(0), static_cast<size_t>(severity));
		return *this;
	}
}