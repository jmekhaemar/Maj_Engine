#ifndef _WCOLOR_INSPECTOR_H_
#define _WCOLOR_INSPECTOR_H_

#include "../WInspectorComponent.h"
#include "Color.h"

namespace maj::editor
{
	class WColorInspector : public WInspectorComponent
	{
	public:
		WColorInspector(QWidget* parent, QFormLayout* layout = nullptr, std::function<void()> callback = nullptr);

		void creatColorDialog();

		template<typename T>
		void load(T* component);

	private:
		void updateRed();
		void updateGreen();
		void updateBlue();
		void updateAlpha();

		Color* mColor;
		uint32_t mR;
		uint32_t mG;
		uint32_t mB;
		uint32_t mA;
	};
}

#include "WColorInspector.inl"

#endif