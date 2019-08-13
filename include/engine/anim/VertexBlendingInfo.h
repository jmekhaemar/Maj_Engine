#ifndef _VERTEX_BLENDING_INFO_H_
#define _VERTEX_BLENDING_INFO_H_

namespace maj::anim
{
	struct VertexBlendingInfo final
	{
	public:
		VertexBlendingInfo(float blendingIndex = 0.0f, float blendingWeight = 0.0f);
		~VertexBlendingInfo() = default;

		bool operator<(const VertexBlendingInfo& rhs);

		float mBlendingIndex;
		float mBlendingWeight;
	};
}

#endif