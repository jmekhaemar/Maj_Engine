#include "anim/AnimVertex.h"

namespace maj::anim
{
	std::vector<rhi::VertexBinding> Vertex::bindings()
	{
		std::vector<rhi::VertexBinding> bindings {};
		bindings.emplace_back(0u, rhi::EVertexInputRate::PER_VERTEX);

		rhi::VertexAttribute vec2Attrib { { rhi::Format::EComponent::R32G32, rhi::Format::EType::SFLOAT }, static_cast<uint32_t>(sizeof(math::Vec2Holder<float>)) };
		rhi::VertexAttribute vec3Attrib { { rhi::Format::EComponent::R32G32B32, rhi::Format::EType::SFLOAT }, static_cast<uint32_t>(sizeof(math::Vec3Holder<float>)) };
		rhi::VertexAttribute vec4Attrib { { rhi::Format::EComponent::R32G32B32A32, rhi::Format::EType::SFLOAT }, static_cast<uint32_t>(sizeof(math::Vec4Holder<float>)) };

		bindings[0u].mAttributes.reserve(6u);
		bindings[0u].mAttributes.emplace_back(vec3Attrib);
		bindings[0u].mAttributes.emplace_back(vec2Attrib);
		bindings[0u].mAttributes.emplace_back(vec3Attrib);
		bindings[0u].mAttributes.emplace_back(vec3Attrib);
		bindings[0u].mAttributes.emplace_back(vec4Attrib);
		bindings[0u].mAttributes.emplace_back(vec4Attrib);

		return bindings;
	}

	Vertex::Vertex(const math::Vec3Holder<float>& kPos, const math::Vec2Holder<float>& kTexCoord, const math::Vec3Holder<float>& kNormal, const math::Vec3Holder<float>& kTan)
		: resources::Vertex { kPos, kTexCoord, kNormal, kTan }
	{}

	Vertex::~Vertex()
	{}

	void Vertex::boneIndex(const math::Vec4Holder<float>& kBoneIndex)
	{
		mBoneIndex = kBoneIndex;
	}	
	
	void Vertex::boneWeight(const math::Vec4Holder<float>& kBoneWeight)
	{
		mBoneWeight = kBoneWeight;
	}

	const math::Vec4Holder<float>& Vertex::boneIndex() const
	{
		return mBoneIndex;
	}

	const math::Vec4Holder<float>& Vertex::boneWeight() const
	{
		return mBoneWeight;
	}

	bool Vertex::operator!=(const Vertex& other) const
	{
		return mPos != other.mPos || mUV != other.mUV || mNormal != other.mNormal || mTangent != other.mTangent || mBoneIndex != other.mBoneIndex || mBoneWeight != other.mBoneWeight;
	}
}