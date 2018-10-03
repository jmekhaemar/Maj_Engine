#include "inspector/custom/WIElementInspector.h"

namespace maj::editor
{
	WIElementInspector::WIElementInspector(QWidget* parent, QFormLayout* layout)
		: WInspectorComponent { parent, layout }
	{}

	void WIElementInspector::onDimensionChanged()
	{
		const math::Vec2f kPos { static_cast<float>(mPosition.mX), static_cast<float>(mPosition.mY) };
		const math::Vec2f kSize { static_cast<float>(mSize.mX), static_cast<float>(mSize.mY) };

		mElement->position(kPos, gui::ECoordFlag::PIXELATED | gui::ECoordFlag::ANCHORED);
		mElement->size(kSize, gui::ECoordFlag::PIXELATED | gui::ECoordFlag::ANCHORED);
	}

	void WIElementInspector::onValueChanged()
	{
		bool isEnable { meta::Field<gui::IElement, 5u>::get(mElement) };
		mElement->mOnEnabled(static_cast<bool&&>(isEnable));
	}
}