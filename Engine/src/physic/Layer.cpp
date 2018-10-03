#include "physic/Layer.h"

namespace maj::physic
{
	DEFINE_HASH_OVERLOAD(LayerFilter)

	void LayerFilter::initialize()
	{}

	std::string LayerFilter::name(const ELayer kValue) const
	{
		if(mLayerNames.count(kValue) != 0u)
			return mLayerNames.at(kValue);

		return {};
	}

	ELayer LayerFilter::filter(const ELayer kValue) const
	{
		if (mLayerFilters.count(kValue) != 0u)
			return mLayerFilters.at(kValue);

		return ELayer::LAYER_1;
	}

	void LayerFilter::name(const ELayer kKey, const char* kName)
	{
		if (mLayerNames.count(kKey) > 0u)
			mLayerNames[kKey] = kName;
	}

	void LayerFilter::filter(const ELayer kKey, const ELayer kValue)
	{
		if (mLayerFilters.count(kKey) > 0u)
			mLayerFilters[kKey] = kValue;
	}
}