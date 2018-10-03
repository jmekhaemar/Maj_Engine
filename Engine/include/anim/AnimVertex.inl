#ifndef _ANIM_VERTEX_INL_
#define _ANIM_VERTEX_INL_

#include "anim/AnimVertex.h"

namespace std
{
	uint64_t hash<maj::anim::Vertex>::operator()(const maj::anim::Vertex& kVertex) const
	{
		return ((hash<maj::math::Vec3Holder<float>>()(kVertex.position()) ^
				(hash<maj::math::Vec2Holder<float>>()(kVertex.uv()) << 1)) >> 1) ^
				(hash<maj::math::Vec3Holder<float>>()(kVertex.normal()) << 1);
	}
}

#endif