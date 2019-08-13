#ifndef _ANIM_VERTEX_H_
#define _ANIM_VERTEX_H_

#include <vector>

#include <Vec4.h>

#include "resources/Vertex.h"

namespace maj::anim
{
	class Vertex : public resources::Vertex
	{
	public:
		static std::vector<rhi::VertexBinding> bindings();

	public:
		Vertex(const math::Vec3Holder<float>& kPos, const math::Vec2Holder<float>& kTexCoord, const math::Vec3Holder<float>& kNormal, const math::Vec3Holder<float>& kTan);
		~Vertex();

		void boneIndex(const math::Vec4Holder<float>& kBoneIndex);
		void boneWeight(const math::Vec4Holder<float>& kBoneWeight);

		const math::Vec4Holder<float>& boneIndex() const;
		const math::Vec4Holder<float>& boneWeight() const;

		bool operator!=(const Vertex& other) const;

	protected:
		math::Vec4Holder<float> mBoneIndex;
		math::Vec4Holder<float> mBoneWeight;
	};
}

namespace std
{
	template<>
	struct hash<maj::anim::Vertex>
	{
		inline uint64_t operator()(const maj::anim::Vertex& kVertex) const;
	};
}

#include "AnimVertex.inl"

#endif