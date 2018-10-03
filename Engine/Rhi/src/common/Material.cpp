#include "common/Material.h"

namespace maj::rhi
{
	DEFINE_HASH_OVERLOAD(LayoutBinding)
	DEFINE_HASH_OVERLOAD(LayoutSet)
	DEFINE_HASH_OVERLOAD(MaterialBinding)
	DEFINE_HASH_OVERLOAD(VertexAttribute)
	DEFINE_HASH_OVERLOAD(VertexBinding)
	DEFINE_HASH_OVERLOAD(DepthBias)
	DEFINE_HASH_OVERLOAD(SampleState)
	DEFINE_HASH_OVERLOAD(DepthBound)
	DEFINE_HASH_OVERLOAD(StencilState)
	DEFINE_HASH_OVERLOAD(BlendState)
	DEFINE_HASH_OVERLOAD(BlendConstant)
	DEFINE_HASH_OVERLOAD(PipelineState)

	LayoutBinding::LayoutBinding(EDescriptorType type, uint32_t count, EShaderStageFlag shaderStage)
		: mType { type }, mCount { count }, mShaderStage { shaderStage }
	{}

	void LayoutBinding::initialize()
	{}

	LayoutSet::LayoutSet(BindingMap&& bindings)
		: mBindings { std::move(bindings) }
	{}

	void LayoutSet::initialize()
	{}

	MaterialBinding::MaterialBinding(uint32_t set, uint32_t binding)
		: mSet { set }, mBinding { binding }
	{}

	void MaterialBinding::initialize()
	{}

	VertexAttribute::VertexAttribute(const Format& kFormat, uint32_t size)
		: mFormatComponent { kFormat.mComponent }, mFormatType { kFormat.mType }, mSize { size }
	{}

	void VertexAttribute::initialize()
	{}

	Format VertexAttribute::format() const
	{
		return { mFormatComponent, mFormatType };
	}

	VertexBinding::VertexBinding(uint32_t binding, EVertexInputRate inputRate)
		: mBinding { binding }, mInputRate { inputRate }, mAttributes {}
	{}

	VertexBinding::VertexBinding(const VertexBinding& kOther)
		: mBinding { kOther.mBinding }, mInputRate { kOther.mInputRate }, mAttributes { kOther.mAttributes }
	{}

	VertexBinding::VertexBinding(VertexBinding&& other)
		: mBinding { other.mBinding }, mInputRate { other.mInputRate }, mAttributes { std::move(other.mAttributes) }
	{}

	void VertexBinding::initialize()
	{}

	VertexBinding& VertexBinding::operator=(const VertexBinding& kOther)
	{
		mBinding = kOther.mBinding;
		mInputRate = kOther.mInputRate;
		mAttributes = kOther.mAttributes;

		return *this;
	}

	VertexBinding& VertexBinding::operator=(VertexBinding&& other)
	{
		mBinding = other.mBinding;
		mInputRate = other.mInputRate;
		mAttributes = std::move(other.mAttributes);

		return *this;
	}

	void DepthBias::initialize()
	{}

	bool DepthBias::isEnabled() const
	{
		return mConstantFactor != 0.f || mClamp != 0.f || mSlopeFactor != 0.f;
	}

	SampleState::SampleState()
		: mSampleCount { ESampleFlag::COUNT_1 }, mMinSampleShading { 1.f }, mSampleMaskEnabled { false }, mSampleMask { 0u }
	{}

	SampleState::SampleState(ESampleFlag sampleCount, float minSampleShading)
		: mSampleCount { sampleCount }, mMinSampleShading { minSampleShading }, mSampleMaskEnabled { false }, mSampleMask { 0u }
	{}

	SampleState::SampleState(ESampleFlag sampleCount, float minSampleShading, uint32_t sampleMask)
		: mSampleCount { sampleCount }, mMinSampleShading { minSampleShading }, mSampleMaskEnabled { true }, mSampleMask { sampleMask }
	{}

	void SampleState::initialize()
	{}

	DepthBound::DepthBound()
		: mMin { 0.f }, mMax { 1.f }
	{}

	DepthBound::DepthBound(float min, float max)
		: mMin { min }, mMax { max }
	{}

	void DepthBound::initialize()
	{}

	bool DepthBound::isEnabled() const
	{
		return mMin != 0.f || mMax != 1.f;
	}

	StencilState::StencilState()
		: mFailOp { EStencilOp::KEEP }, mPassOp { EStencilOp::KEEP }, mDepthFailOp { EStencilOp::KEEP }, mCompareOp { ECompareOp::NEVER }, mCompareMask { 0u }, mWriteMask { 0u },
		mReference { 0u }
	{}

	StencilState::StencilState(EStencilOp failOp, EStencilOp passOp, EStencilOp depthFailOp, ECompareOp compareOp, uint32_t compareMask, uint32_t writeMask, uint32_t reference)
		: mFailOp { failOp }, mPassOp { passOp }, mDepthFailOp { depthFailOp }, mCompareOp { compareOp }, mCompareMask { compareMask }, mWriteMask { writeMask },
		mReference { reference }
	{}

	void StencilState::initialize() {}

	bool StencilState::isEnabled() const
	{
		return mFailOp != EStencilOp::KEEP || mPassOp != EStencilOp::KEEP || mDepthFailOp != EStencilOp::KEEP || mCompareOp != ECompareOp::NEVER || mCompareMask != 0u
			|| mWriteMask != 0u || mReference != 0u;
	}

	BlendState::BlendState()
		: mSrcFactor { EBlendFactor::ONE }, mDstFactor { EBlendFactor::ZERO }, mBlendOp { EBlendOp::ADD }
	{}

	BlendState::BlendState(EBlendFactor srcFactor, EBlendFactor dstFactor, EBlendOp blendOp)
		: mSrcFactor{ srcFactor }, mDstFactor{ dstFactor }, mBlendOp{ blendOp }
	{}

	void BlendState::initialize()
	{}

	bool BlendState::isEnabled() const
	{
		return mSrcFactor != EBlendFactor::ONE || mDstFactor != EBlendFactor::ZERO || mBlendOp != EBlendOp::ADD;
	}

	BlendConstant::BlendConstant(float r, float g, float b, float a)
		: mR{ r }, mG{ g }, mB{ b }, mA{ a }
	{}

	void BlendConstant::initialize()
	{}

	PipelineState::PipelineState()
		: mBindings {}, mTopology { ETopology::TRIANGLE }, mPrimitiveRestartEnabled { false }, mDepthClampEnabled { false }, mPolygonMode { EPolygonMode::FILL },
		mCullMode { ECullModeFlag::BACK }, mDepthBias {}, mSampleState {}, mAlphaToCoverageEnabled { false }, mAlphaToOneEnabled { false }, mDepthTestEnabled { true },
		mDepthWriteEnabled { true }, mDepthCompareOp { ECompareOp::LESS_EQUAL }, mDepthBound {}, mFrontStencil {}, mBackStencil {}, mLogicOp { ELogicOp::COPY }, mColorBlend {},
		mAlphaBlend {}, mColorMask { EColorComponentFlag::R | EColorComponentFlag::G | EColorComponentFlag::B | EColorComponentFlag::A }, mBlendConstants {}
	{}

	PipelineState::PipelineState(const PipelineState& kOther)
		: mBindings { kOther.mBindings }, mTopology { kOther.mTopology }, mPrimitiveRestartEnabled { kOther.mPrimitiveRestartEnabled },
		mDepthClampEnabled { kOther.mDepthClampEnabled }, mPolygonMode { kOther.mPolygonMode }, mCullMode { kOther.mCullMode }, mDepthBias { kOther.mDepthBias },
		mSampleState { kOther.mSampleState }, mAlphaToCoverageEnabled { kOther.mAlphaToCoverageEnabled }, mAlphaToOneEnabled { kOther.mAlphaToOneEnabled },
		mDepthTestEnabled { kOther.mDepthTestEnabled }, mDepthWriteEnabled { kOther.mDepthWriteEnabled }, mDepthCompareOp { kOther.mDepthCompareOp },
		mDepthBound { kOther.mDepthBound }, mFrontStencil { kOther.mFrontStencil }, mBackStencil { kOther.mBackStencil }, mLogicOp { kOther.mLogicOp },
		mColorBlend { kOther.mColorBlend }, mAlphaBlend { kOther.mAlphaBlend }, mColorMask { kOther.mColorMask }, mBlendConstants { kOther.mBlendConstants }
	{}

	PipelineState::PipelineState(PipelineState&& other)
		: mBindings { std::move(other.mBindings) }, mTopology { other.mTopology }, mPrimitiveRestartEnabled { other.mPrimitiveRestartEnabled },
		mDepthClampEnabled { other.mDepthClampEnabled }, mPolygonMode { other.mPolygonMode }, mCullMode { other.mCullMode }, mDepthBias { other.mDepthBias },
		mSampleState { other.mSampleState }, mAlphaToCoverageEnabled { other.mAlphaToCoverageEnabled }, mAlphaToOneEnabled { other.mAlphaToOneEnabled },
		mDepthTestEnabled { other.mDepthTestEnabled }, mDepthWriteEnabled { other.mDepthWriteEnabled }, mDepthCompareOp { other.mDepthCompareOp }, mDepthBound { other.mDepthBound },
		mFrontStencil { other.mFrontStencil }, mBackStencil { other.mBackStencil }, mLogicOp { other.mLogicOp }, mColorBlend { other.mColorBlend }, mAlphaBlend { other.mAlphaBlend },
		mColorMask { other.mColorMask }, mBlendConstants { other.mBlendConstants }
	{}

	void PipelineState::initialize()
	{}

	PipelineState& PipelineState::operator=(const PipelineState& kOther)
	{
		mBindings = kOther.mBindings;

		mTopology = kOther.mTopology;
		mPrimitiveRestartEnabled = kOther.mPrimitiveRestartEnabled;

		mDepthClampEnabled = kOther.mDepthClampEnabled;
		mPolygonMode = kOther.mPolygonMode;
		mCullMode = kOther.mCullMode;
		mDepthBias = kOther.mDepthBias;

		mSampleState = kOther.mSampleState;
		mAlphaToCoverageEnabled = kOther.mAlphaToCoverageEnabled;
		mAlphaToOneEnabled = kOther.mAlphaToOneEnabled;

		mDepthTestEnabled = kOther.mDepthTestEnabled;
		mDepthWriteEnabled = kOther.mDepthWriteEnabled;
		mDepthCompareOp = kOther.mDepthCompareOp;
		mDepthBound = kOther.mDepthBound;
		mFrontStencil = kOther.mFrontStencil;
		mBackStencil = kOther.mBackStencil;

		mLogicOp = kOther.mLogicOp;
		mColorBlend = kOther.mColorBlend;
		mAlphaBlend = kOther.mAlphaBlend;
		mColorMask = kOther.mColorMask;
		mBlendConstants = kOther.mBlendConstants;

		return *this;
	}

	PipelineState& PipelineState::operator=(PipelineState&& other)
	{
		mBindings = std::move(other.mBindings);

		mTopology = other.mTopology;
		mPrimitiveRestartEnabled = other.mPrimitiveRestartEnabled;

		mDepthClampEnabled = other.mDepthClampEnabled;
		mPolygonMode = other.mPolygonMode;
		mCullMode = other.mCullMode;
		mDepthBias = other.mDepthBias;

		mSampleState = other.mSampleState;
		mAlphaToCoverageEnabled = other.mAlphaToCoverageEnabled;
		mAlphaToOneEnabled = other.mAlphaToOneEnabled;

		mDepthTestEnabled = other.mDepthTestEnabled;
		mDepthWriteEnabled = other.mDepthWriteEnabled;
		mDepthCompareOp = other.mDepthCompareOp;
		mDepthBound = other.mDepthBound;
		mFrontStencil = other.mFrontStencil;
		mBackStencil = other.mBackStencil;

		mLogicOp = other.mLogicOp;
		mColorBlend = other.mColorBlend;
		mAlphaBlend = other.mAlphaBlend;
		mColorMask = other.mColorMask;
		mBlendConstants = other.mBlendConstants;

		return *this;
	}
}