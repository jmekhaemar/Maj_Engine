#include "common/ExtensionList.h"

namespace maj::rhi
{
	DEFINE_HASH_OVERLOAD(ExtensionList)

	ExtensionList::ExtensionList()
		: mCubeArray { EExtensionLevel::IGNORED }, mGeometryShader { EExtensionLevel::IGNORED }, mTesselerationShader { EExtensionLevel::IGNORED },
		mSampleRateShading { EExtensionLevel::IGNORED }, mDualSrcBlend { EExtensionLevel::IGNORED }, mLogicOp { EExtensionLevel::IGNORED }, mDepthClamp { EExtensionLevel::IGNORED },
		mDepthBiasClamp { EExtensionLevel::IGNORED }, mPolygonModeNonSolid { EExtensionLevel::IGNORED }, mDepthBounds { EExtensionLevel::IGNORED }, mAlphaToOne { EExtensionLevel::IGNORED },
		mMultiViewport { EExtensionLevel::IGNORED }, mSamplerAnisotropy { EExtensionLevel::IGNORED }, mShaderClipDistance { EExtensionLevel::IGNORED },
		mShaderCullDistance { EExtensionLevel::IGNORED }, mShaderFloat64 { EExtensionLevel::IGNORED }, mShaderInt64 { EExtensionLevel::IGNORED }, mShaderInt16 { EExtensionLevel::IGNORED },
		mShaderResourceMinLOD { EExtensionLevel::IGNORED }, mVariableMultiSamplingRate { EExtensionLevel::IGNORED }
	{}

	void ExtensionList::initialize()
	{}
}