#include "anim/Skeleton.h"
#include "Logging.h"

namespace maj::anim
{
	Skeleton::Skeleton(std::vector<anim::Joint>&& joints)
		: mJoints { std::move(joints) }
	{}

	void Skeleton::addJoint(Joint&& joint)
	{
		mJoints.emplace_back(std::move(joint));
	}

	uint32_t Skeleton::jointCount() const
	{
		return static_cast<uint32_t>(mJoints.size());
	}

	Joint& Skeleton::joint(uint32_t index)
	{
		if (index >= static_cast<uint32_t>(mJoints.size()))
			THROW("failed to get skeleton joint: index out of range")

		return mJoints[index];
	}

	const Joint& Skeleton::joint(uint32_t index) const
	{
		if (index >= static_cast<uint32_t>(mJoints.size()))
			THROW("failed to get skeleton joint: index out of range")

		return mJoints[index];
	}

	uint32_t Skeleton::jointIndexFromName(const char* kName) const
	{
		for (uint32_t i{ 0u }; i < mJoints.size(); ++i)
		{
			if (std::strcmp(mJoints[i].mName, kName) == 0)
				return i;
		}

		THROW("failed to find joint: joint \'" + std::string {kName} + "\' not found")
	}

	void Skeleton::clear()
	{
		mJoints.clear();
	}
}