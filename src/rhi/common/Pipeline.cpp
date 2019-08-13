#include "common/Pipeline.h"

namespace maj::rhi
{
	SubpassDependency::SubpassDependency(SubpassDependencyState&& srcState, SubpassDependencyState&& dstState, bool byRegion)
		: mSrcState { std::move(srcState) }, mDstState { std::move(dstState) }, mByRegion { byRegion }
	{}
}