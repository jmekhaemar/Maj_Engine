#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <vector>

#include <Flag.h>
#include <Vec3.h>
#include <Vec2.h>

#include <common/Material.h>

namespace maj::resources
{
	class Vertex
	{
	public :
		enum class EComponentFlag : uint32_t
		{
			POSITION = 0u,
			UV = 1u,
			NORMAL = 2u,
			UV_NORMAL = UV + NORMAL
		};

		static std::vector<rhi::VertexBinding> bindings();

	public:
		Vertex(const math::Vec3Holder<float>& kPos, const math::Vec2Holder<float>& kTexCoord, const math::Vec3Holder<float>& kNormal, const math::Vec3Holder<float>& kTan = {});
		~Vertex();

		void position(const math::Vec3Holder<float>& kPos);
		void uv(const math::Vec2Holder<float>& kTexCoord);
		void normal(const math::Vec3Holder<float>& kNormal);
		void tangent(const math::Vec3Holder<float>& kTangent);

		const math::Vec3Holder<float>& position() const;
		const math::Vec2Holder<float>& uv() const;
		const math::Vec3Holder<float>& normal() const;
		const math::Vec3Holder<float>& tangent() const;

		bool operator==(const Vertex& other) const;
		bool operator!=(const Vertex& other) const;

	protected:
		math::Vec3Holder<float> mPos;
		math::Vec2Holder<float> mUV;
		math::Vec3Holder<float> mNormal;
		math::Vec3Holder<float> mTangent;
	};

	DECLARE_FLAG(Vertex::EComponentFlag)
}

namespace std
{
	template<>
	struct hash<maj::resources::Vertex>
	{
		inline uint64_t operator()(const maj::resources::Vertex& kVertex) const;
	};
}

#include "resources/Vertex.inl"

META_FLAG(maj::resources::Vertex::EComponentFlag, 4u)
META_FLAG_VALUE(maj::resources::Vertex::EComponentFlag, maj::resources::Vertex::EComponentFlag::POSITION, 0u)
META_FLAG_VALUE(maj::resources::Vertex::EComponentFlag, maj::resources::Vertex::EComponentFlag::UV, 1u)
META_FLAG_VALUE(maj::resources::Vertex::EComponentFlag, maj::resources::Vertex::EComponentFlag::NORMAL, 2u)
META_FLAG_VALUE(maj::resources::Vertex::EComponentFlag, maj::resources::Vertex::EComponentFlag::UV_NORMAL, 3u)

#endif