#ifndef _VERTEX_INL_
#define _VERTEX_INL_

#include "resources/Vertex.h"

namespace maj::resources
{
	DEFINE_FLAG(Vertex::EComponentFlag, uint8_t)
}

namespace std
{
	uint64_t hash<maj::resources::Vertex>::operator()(const maj::resources::Vertex& kVertex) const
	{
		return ((hash<maj::math::Vec3Holder<float>>()(kVertex.position()) ^
				(hash<maj::math::Vec2Holder<float>>()(kVertex.uv()) << 1)) >> 1) ^
				(hash<maj::math::Vec3Holder<float>>()(kVertex.normal()) << 1);
	}
}

#endif