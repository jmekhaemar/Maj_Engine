#include "anim/Joint.h"

namespace maj::anim
{
	Joint::Joint(const char* kName, uint32_t nbChild)
		: mName { kName }, mChildrenIndex {}, mGlobalBind {}
	{
		mChildrenIndex.resize(nbChild);
	}
}