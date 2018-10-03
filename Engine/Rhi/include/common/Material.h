#ifndef _RHI_MATERIAL_H_
#define _RHI_MATERIAL_H_

#include <vector>
#include <map>

#include <ISerializable.h>

#include "common/Format.h"
#include "common/Image.h"
#include "common/Pipeline.h"

namespace maj::rhi
{
	class LayoutBinding final : public ISerializable
	{
		ENABLE_META(LayoutBinding)

	public:
		LOAD_CONSTRUCTOR(LayoutBinding)

		LayoutBinding(EDescriptorType type, uint32_t count, EShaderStageFlag shaderStage);

		EDescriptorType mType;
		uint32_t mCount;
		EShaderStageFlag mShaderStage;
	};

	class LayoutSet final : public ISerializable
	{
		ENABLE_META(LayoutSet)

	public:
		typedef std::map<uint32_t, LayoutBinding> BindingMap;

		LOAD_CONSTRUCTOR(LayoutSet)

		LayoutSet() = default;
		LayoutSet(BindingMap&& bindings);

		BindingMap mBindings;
	};

	class MaterialBinding final : public ISerializable
	{
		ENABLE_META(MaterialBinding)

	public:
		LOAD_CONSTRUCTOR(MaterialBinding)

		MaterialBinding(uint32_t set = 0u, uint32_t binding = 0u);

		uint32_t mSet;
		uint32_t mBinding;
	};

	constexpr bool operator<(const MaterialBinding& kLhs, const MaterialBinding& kRhs);
	constexpr bool operator>(const MaterialBinding& kLhs, const MaterialBinding& kRhs);

	enum class EVertexInputRate
	{
		PER_VERTEX,
		PER_INSTANCE
	};

	struct VertexAttribute final : public ISerializable
	{
		ENABLE_META(VertexAttribute)

	public:
		LOAD_CONSTRUCTOR(VertexAttribute)
		VertexAttribute(const Format& kFormat = { Format::EComponent::UNDEFINED, Format::EType::UNORM }, uint32_t size = 0u);
		~VertexAttribute() = default;

		Format format() const;

		Format::EComponent mFormatComponent;
		Format::EType mFormatType;
		uint32_t mSize;
	};

	struct VertexBinding final : public ISerializable
	{
		ENABLE_META(VertexBinding)

	public:
		LOAD_CONSTRUCTOR(VertexBinding)

		VertexBinding(uint32_t binding = 0u, EVertexInputRate inputRate = EVertexInputRate::PER_VERTEX);
		VertexBinding(const VertexBinding& kOther);
		VertexBinding(VertexBinding&& other);
		~VertexBinding() = default;

		VertexBinding& operator=(const VertexBinding& kOther);
		VertexBinding& operator=(VertexBinding&& other);

		uint32_t mBinding;
		EVertexInputRate mInputRate;

		std::vector<VertexAttribute> mAttributes;
	};

	enum class ETopology
	{
		POINT,
		LINE,
		LINE_STRIP,
		TRIANGLE,
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
		LINE_ADJACENCY,
		LINE_STRIP_ADJACENCY,
		TRIANGLE_ADJACENCY,
		TRIANGLE_STRIP_ADJACENCY,
		PATCH
	};

	enum class EPolygonMode
	{
		FILL,
		LINE,
		POINT
	};

	enum class ECullModeFlag : uint8_t
	{
		NONE = 0u,
		FRONT = 1u,
		BACK = 2u
	};

	struct DepthBias final : public ISerializable
	{
		ENABLE_META(DepthBias)

	public:
		LOAD_CONSTRUCTOR(DepthBias)

		DepthBias() = default;

		bool isEnabled() const;

		float mConstantFactor;
		float mClamp;
		float mSlopeFactor;
	};

	struct SampleState final : public ISerializable
	{
		ENABLE_META(SampleState)

	public:
		LOAD_CONSTRUCTOR(SampleState)

		SampleState();
		SampleState(ESampleFlag sampleCount, float minSampleShading);
		SampleState(ESampleFlag sampleCount, float minSampleShading, uint32_t sampleMask);

		ESampleFlag mSampleCount;
		float mMinSampleShading;
		bool mSampleMaskEnabled;
		uint32_t mSampleMask;
	};

	struct DepthBound final : public ISerializable
	{
		ENABLE_META(DepthBound)

	public:
		LOAD_CONSTRUCTOR(DepthBound)

		DepthBound();
		DepthBound(float min, float max);

		bool isEnabled() const;

		float mMin;
		float mMax;
	};

	// r is reference value, s is stored value
	enum class ECompareOp
	{
		NEVER,				// false
		ALWAYS,				// true
		EQUAL,				// r == s
		NOT_EQUAL,			// r != s
		LESS,				// r < s
		LESS_EQUAL,			// r <= s
		GREATER,			// r > s
		GREATER_EQUAL		// r >= s
	};

	// r is reference value, s is stored value
	enum class EStencilOp
	{
		KEEP,				// s
		ZERO,				// 0
		REPLACE,			// r
		INVERT,				// !s
		INCREMENT_CLAMP,	// max(s + 1, MAX)
		DECREMENT_CLAMP,	// min(s - 1, 0)
		INCREMENT_WRAP,		// s + 1 > MAX ? 0 : s + 1
		DECREMENT_WRAP		// s - 1 < 0 ? MAX : s - 1
	};

	struct StencilState final : public ISerializable
	{
		ENABLE_META(StencilState)

	public:
		LOAD_CONSTRUCTOR(StencilState)

		StencilState();
		StencilState(EStencilOp failOp, EStencilOp passOp, EStencilOp depthFailOp, ECompareOp compareOp, uint32_t compareMask, uint32_t writeMask, uint32_t reference);

		bool isEnabled() const;

		EStencilOp mFailOp;
		EStencilOp mPassOp;
		EStencilOp mDepthFailOp;
		ECompareOp mCompareOp;
		uint32_t mCompareMask;
		uint32_t mWriteMask;
		uint32_t mReference;
	};

	// s is source value, d is destination value
	enum class ELogicOp
	{
		CLEAR,			// 0
		COPY,			// s
		NO_OP,			// d
		COPY_INVERT,	// !s
		INVERT,			// !d
		AND,			// s & d
		OR,				// s | d
		XOR,			// s ^ d
		NAND,			// !(s & d)
		NOR,			// !(s | d)
		NXOR,			// !(s ^ d)
		AND_REVERSE,	// s & !d
		OR_REVERSE,		// s | !d
		AND_INVERT,		// !s & d
		OR_INVERT,		// !s| d
		SET				// 0xFFFFFF
	};

	// s is source value, d is destination value, b is blend constant, t is second source value
	enum class EBlendFactor
	{
		ZERO,					// 0
		ONE,					// 1
		SRC_RGB,				// s
		SRC_A,					// s
		SRC_A_SATURATE,			// min(s, 1 - d) or 1 if alpha blend factor
		DST_RGB,				// d
		DST_A,					// d
		CONSTANT_RGB,			// b
		CONSTANT_A,				// b
		SRC1_RGB,				// t
		SRC1_A,					// t
		ONE_MINUS_SRC_RGB,		// 1 - s
		ONE_MINUS_SRC_A,		// 1 - s
		ONE_MINUS_DST_RGB,		// 1 - d
		ONE_MINUS_DST_A,		// 1 - d
		ONE_MINUS_CONSTANT_RGB,	// 1 - b
		ONE_MINUS_CONSTANT_A,	// 1 - b
		ONE_MINUS_SRC1_RGB,		// 1 - t
		ONE_MINUS_SRC1_A		// 1 - t
	};

	// s is source value, d is destination value, K is blend factor
	enum class EBlendOp
	{
		ADD,				// s * Ks + d * Kd
		SUBTRACT,			// s * Ks - d * Kd
		REVERSE_SUBTRACT,	// d * Kd - s * Ks
		MIN,				// min(s, d)
		MAX					// max(s, d)
	};

	struct BlendState final : public ISerializable
	{
		ENABLE_META(BlendState)

	public:
		LOAD_CONSTRUCTOR(BlendState)

		BlendState();
		BlendState(EBlendFactor srcFactor, EBlendFactor dstFactor, EBlendOp blendOp);

		bool isEnabled() const;

		EBlendFactor mSrcFactor;
		EBlendFactor mDstFactor;
		EBlendOp mBlendOp;
	};

	enum class EColorComponentFlag : uint8_t
	{
		R = 1u,
		G = 2u,
		B = 4u,
		A = 8u
	};

	struct BlendConstant : public ISerializable
	{
		ENABLE_META(BlendConstant)

	public:
		LOAD_CONSTRUCTOR(BlendConstant)

		BlendConstant() = default;
		BlendConstant(float r, float g, float b, float a);

		float mR;
		float mG;
		float mB;
		float mA;
	};

	// TODO Add tesseleration support
	struct PipelineState final : public ISerializable
	{
		ENABLE_META(PipelineState)

	public:
		LOAD_CONSTRUCTOR(PipelineState)

		PipelineState();
		PipelineState(const PipelineState& kOther);
		PipelineState(PipelineState&& other);

		PipelineState& operator=(const PipelineState& kOther);
		PipelineState& operator=(PipelineState&& other);

		std::vector<VertexBinding> mBindings;

		ETopology mTopology;
		bool mPrimitiveRestartEnabled;

		bool mDepthClampEnabled;
		EPolygonMode mPolygonMode;
		ECullModeFlag mCullMode;
		DepthBias mDepthBias;

		SampleState mSampleState;
		bool mAlphaToCoverageEnabled;
		bool mAlphaToOneEnabled;

		bool mDepthTestEnabled;
		bool mDepthWriteEnabled;
		ECompareOp mDepthCompareOp;
		DepthBound mDepthBound;
		StencilState mFrontStencil;
		StencilState mBackStencil;

		ELogicOp mLogicOp;
		BlendState mColorBlend;
		BlendState mAlphaBlend;
		EColorComponentFlag mColorMask;
		BlendConstant mBlendConstants;
	};

	DECLARE_FLAG(ECullModeFlag)
	DECLARE_FLAG(EColorComponentFlag)
}

#include "common/Material.inl"

META_CLASS(maj::rhi::LayoutBinding, 3u)
META_FIELD(maj::rhi::LayoutBinding, maj::rhi::EDescriptorType, mType, 0u)
META_FIELD(maj::rhi::LayoutBinding, uint32_t, mCount, 1u)
META_FIELD(maj::rhi::LayoutBinding, maj::rhi::EShaderStageFlag, mShaderStage, 2u)

META_CLASS(maj::rhi::LayoutSet, 1u)
META_FIELD_MAP(maj::rhi::LayoutSet, maj::rhi::LayoutSet::BindingMap, mBindings, 0u)

META_CLASS(maj::rhi::MaterialBinding, 2u)
META_FIELD(maj::rhi::MaterialBinding, uint32_t, mSet, 0u)
META_FIELD(maj::rhi::MaterialBinding, uint32_t, mBinding, 1u)

META_CLASS(maj::rhi::VertexAttribute, 3u)
META_FIELD(maj::rhi::VertexAttribute, maj::rhi::Format::EComponent, mFormatComponent, 0u)
META_FIELD(maj::rhi::VertexAttribute, maj::rhi::Format::EType, mFormatType, 1u)
META_FIELD(maj::rhi::VertexAttribute, uint32_t, mSize, 2u)

META_ENUM(maj::rhi::EVertexInputRate, 2u)
META_ENUM_VALUE(maj::rhi::EVertexInputRate, maj::rhi::EVertexInputRate::PER_VERTEX)
META_ENUM_VALUE(maj::rhi::EVertexInputRate, maj::rhi::EVertexInputRate::PER_INSTANCE)

META_CLASS(maj::rhi::VertexBinding, 3u)
META_FIELD(maj::rhi::VertexBinding, uint32_t, mBinding, 0u)
META_FIELD(maj::rhi::VertexBinding, maj::rhi::EVertexInputRate, mInputRate, 1u)
META_FIELD_ARRAY(maj::rhi::VertexBinding, std::vector<maj::rhi::VertexAttribute>, mAttributes, 2u)

META_ENUM(maj::rhi::ETopology, 11u)
META_ENUM_VALUE(maj::rhi::ETopology, maj::rhi::ETopology::POINT)
META_ENUM_VALUE(maj::rhi::ETopology, maj::rhi::ETopology::LINE)
META_ENUM_VALUE(maj::rhi::ETopology, maj::rhi::ETopology::LINE_STRIP)
META_ENUM_VALUE(maj::rhi::ETopology, maj::rhi::ETopology::TRIANGLE)
META_ENUM_VALUE(maj::rhi::ETopology, maj::rhi::ETopology::TRIANGLE_STRIP)
META_ENUM_VALUE(maj::rhi::ETopology, maj::rhi::ETopology::TRIANGLE_FAN)
META_ENUM_VALUE(maj::rhi::ETopology, maj::rhi::ETopology::LINE_ADJACENCY)
META_ENUM_VALUE(maj::rhi::ETopology, maj::rhi::ETopology::LINE_STRIP_ADJACENCY)
META_ENUM_VALUE(maj::rhi::ETopology, maj::rhi::ETopology::TRIANGLE_ADJACENCY)
META_ENUM_VALUE(maj::rhi::ETopology, maj::rhi::ETopology::TRIANGLE_STRIP_ADJACENCY)
META_ENUM_VALUE(maj::rhi::ETopology, maj::rhi::ETopology::PATCH)

META_FLAG(maj::rhi::ECullModeFlag, 3u)
META_FLAG_VALUE(maj::rhi::ECullModeFlag, maj::rhi::ECullModeFlag::NONE, 0u)
META_FLAG_VALUE(maj::rhi::ECullModeFlag, maj::rhi::ECullModeFlag::FRONT, 1u)
META_FLAG_VALUE(maj::rhi::ECullModeFlag, maj::rhi::ECullModeFlag::BACK, 2u)

META_ENUM(maj::rhi::EPolygonMode, 3u)
META_ENUM_VALUE(maj::rhi::EPolygonMode, maj::rhi::EPolygonMode::FILL)
META_ENUM_VALUE(maj::rhi::EPolygonMode, maj::rhi::EPolygonMode::LINE)
META_ENUM_VALUE(maj::rhi::EPolygonMode, maj::rhi::EPolygonMode::POINT)

META_CLASS(maj::rhi::DepthBias, 3u)
META_FIELD(maj::rhi::DepthBias, float, mConstantFactor, 0u)
META_FIELD(maj::rhi::DepthBias, float, mClamp, 1u)
META_FIELD(maj::rhi::DepthBias, float, mSlopeFactor, 2u)

META_CLASS(maj::rhi::SampleState, 4u)
META_FIELD(maj::rhi::SampleState, maj::rhi::ESampleFlag, mSampleCount, 0u)
META_FIELD(maj::rhi::SampleState, float, mMinSampleShading, 1u)
META_FIELD(maj::rhi::SampleState, bool, mSampleMaskEnabled, 2u)
META_FIELD(maj::rhi::SampleState, uint32_t, mSampleMask, 3u)

META_CLASS(maj::rhi::DepthBound, 2u)
META_FIELD(maj::rhi::DepthBound, float, mMin, 0u)
META_FIELD(maj::rhi::DepthBound, float, mMax, 1u)

META_ENUM(maj::rhi::EStencilOp, 8u)
META_ENUM_VALUE(maj::rhi::EStencilOp, maj::rhi::EStencilOp::KEEP)
META_ENUM_VALUE(maj::rhi::EStencilOp, maj::rhi::EStencilOp::ZERO)
META_ENUM_VALUE(maj::rhi::EStencilOp, maj::rhi::EStencilOp::REPLACE)
META_ENUM_VALUE(maj::rhi::EStencilOp, maj::rhi::EStencilOp::INVERT)
META_ENUM_VALUE(maj::rhi::EStencilOp, maj::rhi::EStencilOp::INCREMENT_CLAMP)
META_ENUM_VALUE(maj::rhi::EStencilOp, maj::rhi::EStencilOp::DECREMENT_CLAMP)
META_ENUM_VALUE(maj::rhi::EStencilOp, maj::rhi::EStencilOp::INCREMENT_WRAP)
META_ENUM_VALUE(maj::rhi::EStencilOp, maj::rhi::EStencilOp::DECREMENT_WRAP)

META_ENUM(maj::rhi::ECompareOp, 8u)
META_ENUM_VALUE(maj::rhi::ECompareOp, maj::rhi::ECompareOp::NEVER)
META_ENUM_VALUE(maj::rhi::ECompareOp, maj::rhi::ECompareOp::ALWAYS)
META_ENUM_VALUE(maj::rhi::ECompareOp, maj::rhi::ECompareOp::EQUAL)
META_ENUM_VALUE(maj::rhi::ECompareOp, maj::rhi::ECompareOp::NOT_EQUAL)
META_ENUM_VALUE(maj::rhi::ECompareOp, maj::rhi::ECompareOp::LESS)
META_ENUM_VALUE(maj::rhi::ECompareOp, maj::rhi::ECompareOp::LESS_EQUAL)
META_ENUM_VALUE(maj::rhi::ECompareOp, maj::rhi::ECompareOp::GREATER)
META_ENUM_VALUE(maj::rhi::ECompareOp, maj::rhi::ECompareOp::GREATER_EQUAL)

META_CLASS(maj::rhi::StencilState, 7u)
META_FIELD(maj::rhi::StencilState, maj::rhi::EStencilOp, mFailOp, 0u)
META_FIELD(maj::rhi::StencilState, maj::rhi::EStencilOp, mPassOp, 1u)
META_FIELD(maj::rhi::StencilState, maj::rhi::EStencilOp, mDepthFailOp, 2u)
META_FIELD(maj::rhi::StencilState, maj::rhi::ECompareOp, mCompareOp, 3u)
META_FIELD(maj::rhi::StencilState, uint32_t, mCompareMask, 4u)
META_FIELD(maj::rhi::StencilState, uint32_t, mWriteMask, 5u)
META_FIELD(maj::rhi::StencilState, uint32_t, mReference, 6u)

META_ENUM(maj::rhi::ELogicOp, 16u)
META_ENUM_VALUE(maj::rhi::ELogicOp, maj::rhi::ELogicOp::CLEAR)
META_ENUM_VALUE(maj::rhi::ELogicOp, maj::rhi::ELogicOp::COPY)
META_ENUM_VALUE(maj::rhi::ELogicOp, maj::rhi::ELogicOp::NO_OP)
META_ENUM_VALUE(maj::rhi::ELogicOp, maj::rhi::ELogicOp::COPY_INVERT)
META_ENUM_VALUE(maj::rhi::ELogicOp, maj::rhi::ELogicOp::INVERT)
META_ENUM_VALUE(maj::rhi::ELogicOp, maj::rhi::ELogicOp::AND)
META_ENUM_VALUE(maj::rhi::ELogicOp, maj::rhi::ELogicOp::OR)
META_ENUM_VALUE(maj::rhi::ELogicOp, maj::rhi::ELogicOp::XOR)
META_ENUM_VALUE(maj::rhi::ELogicOp, maj::rhi::ELogicOp::NAND)
META_ENUM_VALUE(maj::rhi::ELogicOp, maj::rhi::ELogicOp::NOR)
META_ENUM_VALUE(maj::rhi::ELogicOp, maj::rhi::ELogicOp::NXOR)
META_ENUM_VALUE(maj::rhi::ELogicOp, maj::rhi::ELogicOp::AND_REVERSE)
META_ENUM_VALUE(maj::rhi::ELogicOp, maj::rhi::ELogicOp::OR_REVERSE)
META_ENUM_VALUE(maj::rhi::ELogicOp, maj::rhi::ELogicOp::AND_INVERT)
META_ENUM_VALUE(maj::rhi::ELogicOp, maj::rhi::ELogicOp::OR_INVERT)
META_ENUM_VALUE(maj::rhi::ELogicOp, maj::rhi::ELogicOp::SET)

META_ENUM(maj::rhi::EBlendFactor, 19u)
META_ENUM_VALUE(maj::rhi::EBlendFactor, maj::rhi::EBlendFactor::ZERO)
META_ENUM_VALUE(maj::rhi::EBlendFactor, maj::rhi::EBlendFactor::ONE)
META_ENUM_VALUE(maj::rhi::EBlendFactor, maj::rhi::EBlendFactor::SRC_RGB)
META_ENUM_VALUE(maj::rhi::EBlendFactor, maj::rhi::EBlendFactor::SRC_A)
META_ENUM_VALUE(maj::rhi::EBlendFactor, maj::rhi::EBlendFactor::SRC_A_SATURATE)
META_ENUM_VALUE(maj::rhi::EBlendFactor, maj::rhi::EBlendFactor::DST_RGB)
META_ENUM_VALUE(maj::rhi::EBlendFactor, maj::rhi::EBlendFactor::DST_A)
META_ENUM_VALUE(maj::rhi::EBlendFactor, maj::rhi::EBlendFactor::CONSTANT_RGB)
META_ENUM_VALUE(maj::rhi::EBlendFactor, maj::rhi::EBlendFactor::CONSTANT_A)
META_ENUM_VALUE(maj::rhi::EBlendFactor, maj::rhi::EBlendFactor::SRC1_RGB)
META_ENUM_VALUE(maj::rhi::EBlendFactor, maj::rhi::EBlendFactor::SRC1_A)
META_ENUM_VALUE(maj::rhi::EBlendFactor, maj::rhi::EBlendFactor::ONE_MINUS_SRC_RGB)
META_ENUM_VALUE(maj::rhi::EBlendFactor, maj::rhi::EBlendFactor::ONE_MINUS_SRC_A)
META_ENUM_VALUE(maj::rhi::EBlendFactor, maj::rhi::EBlendFactor::ONE_MINUS_DST_RGB)
META_ENUM_VALUE(maj::rhi::EBlendFactor, maj::rhi::EBlendFactor::ONE_MINUS_DST_A)
META_ENUM_VALUE(maj::rhi::EBlendFactor, maj::rhi::EBlendFactor::ONE_MINUS_CONSTANT_RGB)
META_ENUM_VALUE(maj::rhi::EBlendFactor, maj::rhi::EBlendFactor::ONE_MINUS_CONSTANT_A)
META_ENUM_VALUE(maj::rhi::EBlendFactor, maj::rhi::EBlendFactor::ONE_MINUS_SRC1_RGB)
META_ENUM_VALUE(maj::rhi::EBlendFactor, maj::rhi::EBlendFactor::ONE_MINUS_SRC1_A)

META_ENUM(maj::rhi::EBlendOp, 5u)
META_ENUM_VALUE(maj::rhi::EBlendOp, maj::rhi::EBlendOp::ADD)
META_ENUM_VALUE(maj::rhi::EBlendOp, maj::rhi::EBlendOp::SUBTRACT)
META_ENUM_VALUE(maj::rhi::EBlendOp, maj::rhi::EBlendOp::REVERSE_SUBTRACT)
META_ENUM_VALUE(maj::rhi::EBlendOp, maj::rhi::EBlendOp::MIN)
META_ENUM_VALUE(maj::rhi::EBlendOp, maj::rhi::EBlendOp::MAX)

META_CLASS(maj::rhi::BlendState, 3u)
META_FIELD(maj::rhi::BlendState, maj::rhi::EBlendFactor, mSrcFactor, 0u)
META_FIELD(maj::rhi::BlendState, maj::rhi::EBlendFactor, mDstFactor, 1u)
META_FIELD(maj::rhi::BlendState, maj::rhi::EBlendOp, mBlendOp, 2u)

META_FLAG(maj::rhi::EColorComponentFlag, 4u)
META_FLAG_VALUE(maj::rhi::EColorComponentFlag, maj::rhi::EColorComponentFlag::R, 0u)
META_FLAG_VALUE(maj::rhi::EColorComponentFlag, maj::rhi::EColorComponentFlag::G, 1u)
META_FLAG_VALUE(maj::rhi::EColorComponentFlag, maj::rhi::EColorComponentFlag::B, 2u)
META_FLAG_VALUE(maj::rhi::EColorComponentFlag, maj::rhi::EColorComponentFlag::A, 3u)

META_CLASS(maj::rhi::BlendConstant, 4u)
META_FIELD(maj::rhi::BlendConstant, float, mR, 0u)
META_FIELD(maj::rhi::BlendConstant, float, mG, 1u)
META_FIELD(maj::rhi::BlendConstant, float, mB, 2u)
META_FIELD(maj::rhi::BlendConstant, float, mA, 3u)

META_CLASS(maj::rhi::PipelineState, 21u)
META_FIELD_ARRAY(maj::rhi::PipelineState, std::vector<maj::rhi::VertexBinding>, mBindings, 0u)
META_FIELD(maj::rhi::PipelineState, maj::rhi::ETopology, mTopology, 1u)
META_FIELD(maj::rhi::PipelineState, bool, mPrimitiveRestartEnabled, 2u)
META_FIELD(maj::rhi::PipelineState, bool, mDepthClampEnabled, 3u)
META_FIELD(maj::rhi::PipelineState, maj::rhi::EPolygonMode, mPolygonMode, 4u)
META_FIELD(maj::rhi::PipelineState, maj::rhi::ECullModeFlag, mCullMode, 5u)
META_FIELD(maj::rhi::PipelineState, maj::rhi::DepthBias, mDepthBias, 6u)
META_FIELD(maj::rhi::PipelineState, maj::rhi::SampleState, mSampleState, 7u)
META_FIELD(maj::rhi::PipelineState, bool, mAlphaToCoverageEnabled, 8u)
META_FIELD(maj::rhi::PipelineState, bool, mAlphaToOneEnabled, 9u)
META_FIELD(maj::rhi::PipelineState, bool, mDepthTestEnabled, 10u)
META_FIELD(maj::rhi::PipelineState, bool, mDepthWriteEnabled, 11u)
META_FIELD(maj::rhi::PipelineState, maj::rhi::ECompareOp, mDepthCompareOp, 12u)
META_FIELD(maj::rhi::PipelineState, maj::rhi::DepthBound, mDepthBound, 13u)
META_FIELD(maj::rhi::PipelineState, maj::rhi::StencilState, mFrontStencil, 14u)
META_FIELD(maj::rhi::PipelineState, maj::rhi::StencilState, mBackStencil, 15u)
META_FIELD(maj::rhi::PipelineState, maj::rhi::ELogicOp, mLogicOp, 16u)
META_FIELD(maj::rhi::PipelineState, maj::rhi::BlendState, mColorBlend, 17u)
META_FIELD(maj::rhi::PipelineState, maj::rhi::BlendState, mAlphaBlend, 18u)
META_FIELD(maj::rhi::PipelineState, maj::rhi::EColorComponentFlag, mColorMask, 19u)
META_FIELD(maj::rhi::PipelineState, maj::rhi::BlendConstant, mBlendConstants, 20u)

#endif