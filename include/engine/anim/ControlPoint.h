#ifndef _CONTROL_POINT_H_
#define _CONTROL_POINT_H_

#include <vector>

#include <Vec3.h>

#include "anim/VertexBlendingInfo.h"

namespace maj::anim
{
	struct ControlPoint final
	{
	public:
		static const uint32_t skMaxBlendingInfo;

		ControlPoint();
		ControlPoint(ControlPoint&& other);
		~ControlPoint() = default;
	
		math::Vec3Holder<float> mPosition;
		std::vector<anim::VertexBlendingInfo> mBlendingInfo;
	};
}

#endif