#ifndef _FRAME_GRAPH_INL_
#define _FRAME_GRAPH_INL_

#include "FrameGraph.h"

namespace maj::rhi
{
	template <typename... Args>
	void FrameGraph::add(const MaterialInstance* kMaterialInstance, Args&&... args)
	{
		if (!kMaterialInstance)
			return;

		// Make search key
		const Material& kMaterial { kMaterialInstance->material() };
		const Material::Info& kInfo { kMaterial.info() };

		uint32_t renderPassIdx { renderPassIndex(kInfo.renderPass()) };
		uint32_t materialIdx { materialIndex(kMaterial, renderPassIdx) };

		Key key { materialIdx, kInfo.mSortingOrder, !kInfo.mPipelineState.mDepthTestEnabled, kInfo.mSubpassIndex, renderPassIdx };

		// Make draw call
		DrawCall temp { kMaterialInstance, std::forward<Args>(args)... };
		mDrawCalls.emplace(key, std::move(temp));

		// If there already is a command buffer, tell it to be updated
		if (mCommands.count(key) != 0u)
			mCommands.at(key).mIsDirty = true;

		mIsDirty = true;
	}
}

#endif