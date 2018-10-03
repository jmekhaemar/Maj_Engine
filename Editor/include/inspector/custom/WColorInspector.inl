#ifndef _WCOLOR_INSPECTOR_INL_
#define _WCOLOR_INSPECTOR_INL_

#include <QBoxLayout>
#include <QPushButton>

#include "WColorInspector.h"

namespace maj::editor
{
	template<typename T>
	void WColorInspector::load(T* component)
	{
		std::string name{ meta::Class<T>::name() };
		setObjectName(name.substr(name.rfind(':') + 1u).c_str());

		mColor =  static_cast<Color*>(component);
		mR = static_cast<uint32_t>(mColor->r());
		mG = static_cast<uint32_t>(mColor->g());
		mB = static_cast<uint32_t>(mColor->b());
		mA = static_cast<uint32_t>(mColor->a());

		QHBoxLayout* layout{ new QHBoxLayout{} };

		{
			QFormLayout* formLayout{ new QFormLayout{} };
			WIntSpinBox* widget{ new WIntSpinBox{ &mR, this,  std::bind(&WColorInspector::updateRed,this) } };
			widget->setRange(0, 255);
			formLayout->addRow("R", widget);
			layout->addLayout(formLayout);
		}
		{
			QFormLayout* formLayout{ new QFormLayout{} };
			WIntSpinBox* widget{ new WIntSpinBox{ &mG, this, std::bind(&WColorInspector::updateGreen,this) } };
			widget->setRange(0, 255);
			formLayout->addRow("G", widget);
			layout->addLayout(formLayout);
		}
		{
			QFormLayout* formLayout{ new QFormLayout{} };
			WIntSpinBox* widget{ new WIntSpinBox{ &mB, this, std::bind(&WColorInspector::updateBlue,this) } };
			widget->setRange(0, 255);
			formLayout->addRow("B", widget);
			layout->addLayout(formLayout);
		}
		{
			QFormLayout* formLayout{ new QFormLayout{} };
			WIntSpinBox* widget{ new WIntSpinBox{ &mA, this, std::bind(&WColorInspector::updateAlpha,this) } };
			widget->setRange(0, 255);
			formLayout->addRow("A", widget);
			layout->addLayout(formLayout);
		}
		{
			QPushButton* button = new QPushButton("Pick", this);
			button->setMaximumWidth(40);
			button->connect(button, &QPushButton::pressed, this, [this]()->void {creatColorDialog(); });
			layout->addWidget(button);
		}

		mLayout->addRow(mLayout->objectName(), layout);
	}
}

#endif