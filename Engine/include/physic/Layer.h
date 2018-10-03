#ifndef _LAYER_H_
#define _LAYER_H_

#include <unordered_map>

#include <ISerializable.h>
#include <Flag.h>

namespace maj::physic
{
	enum class ELayer : uint8_t
	{
		LAYER_1 = 1u,
		LAYER_2 = 2u,
		LAYER_3 = 4u,
		LAYER_4 = 8u,
		LAYER_5 = 16u,
		LAYER_6 = 32u,
		LAYER_7 = 64u,
		LAYER_8 = 128u
	};

	typedef std::unordered_map<ELayer, ELayer> LayerFilterType;
	typedef std::unordered_map<ELayer, std::string> LayerName;

	struct LayerFilter final : public ISerializable
	{
		ENABLE_META(LayerFilter)

	public:
		LOAD_CONSTRUCTOR(LayerFilter)
		LayerFilter() = default;
		~LayerFilter() = default;

		std::string name(const ELayer kValue) const;
		ELayer filter(const ELayer kValue) const;

		void name(const ELayer kKey, const char* kName);
		void filter(const ELayer kKey, const ELayer kValue);

	private:
		LayerName mLayerNames;
		LayerFilterType mLayerFilters;
	};

	DECLARE_FLAG(ELayer)
	DEFINE_FLAG(ELayer, uint8_t)
}

META_CLASS(maj::physic::LayerFilter, 2u)
META_FIELD_MAP(maj::physic::LayerFilter, maj::physic::LayerName, mLayerNames, 0u)
META_FIELD_MAP(maj::physic::LayerFilter, maj::physic::LayerFilterType, mLayerFilters, 1u)

META_FLAG(maj::physic::ELayer, 8u)
META_FLAG_VALUE(maj::physic::ELayer, maj::physic::ELayer::LAYER_1, 0u)
META_FLAG_VALUE(maj::physic::ELayer, maj::physic::ELayer::LAYER_2, 1u)
META_FLAG_VALUE(maj::physic::ELayer, maj::physic::ELayer::LAYER_3, 2u)
META_FLAG_VALUE(maj::physic::ELayer, maj::physic::ELayer::LAYER_4, 3u)
META_FLAG_VALUE(maj::physic::ELayer, maj::physic::ELayer::LAYER_5, 4u)
META_FLAG_VALUE(maj::physic::ELayer, maj::physic::ELayer::LAYER_6, 5u)
META_FLAG_VALUE(maj::physic::ELayer, maj::physic::ELayer::LAYER_7, 6u)
META_FLAG_VALUE(maj::physic::ELayer, maj::physic::ELayer::LAYER_8, 7u)

#endif