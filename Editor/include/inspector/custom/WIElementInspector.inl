#ifndef _WIELEMENT_INSPECTOR_INL_
#define _WIELEMENT_INSPECTOR_INL_

#include "inspector/custom/WIElementInspector.h"

#include "inspector/WFlagInspector.h"
#include "inspector/custom/WVec2uiInspector.h"

namespace maj::editor
{
	template <typename T>
	void WIElementInspector::load(T* component)
	{
		mElement = static_cast<gui::IElement*>(component);

		if constexpr (meta::Class<T>::hasParent())
			loadParent<typename meta::Class<T>::Parent>(component);

		std::string name { meta::Class<T>::name() };
		setObjectName(name.substr(name.rfind(':') + 1u).c_str());

		{
			WFlagInspector<typename meta::Field<T, 0u>::Type>* widget { new WFlagInspector { meta::Field<T, 0u>::get(mElement), this } };
			std::string subStr { meta::Field<T, 0u>::name() };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}

		{
			WFlagInspector<typename meta::Field<T, 1u>::Type>* widget { new WFlagInspector { meta::Field<T, 1u>::get(mElement), this } };
			std::string subStr { meta::Field<T, 1u>::name() };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}

		{
			math::Vec2f pos { mElement->position(gui::ECoordFlag::PIXELATED | gui::ECoordFlag::ANCHORED) };
			mPosition.mX = static_cast<uint32_t>(pos.mX);
			mPosition.mY = static_cast<uint32_t>(pos.mY);

			WVec2uiInspector* widget { new WVec2uiInspector { this, nullptr, std::bind(&WIElementInspector::onDimensionChanged, this) } };
			widget->load<math::Vec2ui>(&mPosition);
			std::string subStr { meta::Field<T, 2u>::name() };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}

		{
			math::Vec2f size { mElement->size(gui::ECoordFlag::PIXELATED) };
			mSize.mX = static_cast<uint32_t>(size.mX);
			mSize.mY = static_cast<uint32_t>(size.mY);

			WVec2uiInspector* widget { new WVec2uiInspector { this, nullptr, std::bind(&WIElementInspector::onDimensionChanged, this) } };
			widget->load<math::Vec2ui>(&mSize);
			std::string subStr { meta::Field<T, 3u>::name() };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}

		{
			WColorInspector* widget { new WColorInspector { this } };
			widget->load<Color>(&meta::Field<T, 4u>::get(mElement));
			std::string subStr { meta::Field<T, 4u>::name() };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}

		{
			WCheckBox* widget { new WCheckBox { meta::Field<T, 5u>::get(mElement), this, std::bind(&WIElementInspector::onValueChanged, this) } };
			std::string subStr { meta::Field<T, 5u>::name() };
			mLayout->addRow(subStr.substr(1).c_str(), widget);
		}
	}
}

#endif