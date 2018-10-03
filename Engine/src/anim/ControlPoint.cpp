#include "anim/ControlPoint.h"
	
namespace maj::anim
{
	const uint32_t ControlPoint::skMaxBlendingInfo { 4u };

	ControlPoint::ControlPoint()
		: mPosition {}, mBlendingInfo {}
	{
		mBlendingInfo.reserve(skMaxBlendingInfo);
	}

	ControlPoint::ControlPoint(ControlPoint&& other)
		: mPosition { other.mPosition }, mBlendingInfo { std::move(other.mBlendingInfo) }
	{}
}