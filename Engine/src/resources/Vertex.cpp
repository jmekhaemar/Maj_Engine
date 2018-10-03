#include "resources/Vertex.h"

namespace maj::resources
{
	std::vector<rhi::VertexBinding> Vertex::bindings()
	{
		std::vector<rhi::VertexBinding> bindings {};
		bindings.emplace_back(0u, rhi::EVertexInputRate::PER_VERTEX);

		rhi::VertexAttribute vec2Attrib { { rhi::Format::EComponent::R32G32, rhi::Format::EType::SFLOAT }, static_cast<uint32_t>(sizeof(math::Vec2Holder<float>)) };
		rhi::VertexAttribute vec3Attrib { { rhi::Format::EComponent::R32G32B32, rhi::Format::EType::SFLOAT }, static_cast<uint32_t>(sizeof(math::Vec3Holder<float>)) };

		bindings[0u].mAttributes.reserve(4u);
		bindings[0u].mAttributes.emplace_back(vec3Attrib);
		bindings[0u].mAttributes.emplace_back(vec2Attrib);
		bindings[0u].mAttributes.emplace_back(vec3Attrib);
		bindings[0u].mAttributes.emplace_back(vec3Attrib);

		return bindings;
	}

	Vertex::Vertex(const math::Vec3Holder<float>& kPos, const math::Vec2Holder<float>& kTexCoord, const math::Vec3Holder<float>& kNormal, const math::Vec3Holder<float>& kTan)
		:mPos{ kPos }, mUV{ kTexCoord }, mNormal{ kNormal }, mTangent { kTan }
	{}

	Vertex::~Vertex()
	{}

	void Vertex::position(const math::Vec3Holder<float>& kPos)
	{
		mPos = kPos;
	}

	void Vertex::uv(const math::Vec2Holder<float>& kTexCoord)
	{
		mUV =  kTexCoord;
	}

	void Vertex::normal(const math::Vec3Holder<float>& kNormal)
	{
		mNormal = kNormal;
	}

	void Vertex::tangent(const math::Vec3Holder<float>& kTangent)
	{
		mTangent = kTangent;
	}

	const math::Vec3Holder<float>& Vertex::position() const
	{
		return mPos;
	}

	const math::Vec2Holder<float>& Vertex::uv() const
	{
		return mUV;
	}

	const math::Vec3Holder<float>& Vertex::normal() const
	{
		return mNormal;
	}

	const math::Vec3Holder<float>& Vertex::tangent() const
	{
		return mTangent;
	}

	bool Vertex::operator==(const Vertex& other) const
	{
		return !(*this != other);
	}

	bool Vertex::operator!=(const Vertex& other) const
	{
		return mPos != other.mPos || mUV != other.mUV || mNormal != other.mNormal || mTangent != other.mTangent;
	}
}