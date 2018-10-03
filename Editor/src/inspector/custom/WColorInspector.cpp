#include "inspector/custom/WColorInspector.h"

#include <QColorDialog>
#include <QColor>

namespace maj::editor
{
	WColorInspector::WColorInspector(QWidget* parent, QFormLayout* layout, std::function<void()> callback)
		: WInspectorComponent{ parent, layout, callback }
	{
	}

	void WColorInspector::creatColorDialog()
	{
		QColorDialog* dial =  new QColorDialog(QColor( mR, mG, mB));

		dial->setWindowFlags(dial->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		dial->setWindowModality(Qt::ApplicationModal);

		dial->connect(dial, &QColorDialog::accepted, this, [this, dial]()->void
		{
			mR = dial->currentColor().red();
			mG = dial->currentColor().green();
			mB = dial->currentColor().blue();
			mA = dial->currentColor().alpha();
			updateRed();
			updateGreen();
			updateBlue();
			updateAlpha();
		});
		dial->show();
	}

	void WColorInspector::updateRed()
	{
		mColor->r(mR);
	}

	void WColorInspector::updateGreen()
	{
		mColor->g(mG);
	}

	void WColorInspector::updateBlue() 
	{
		mColor->b(mB);
	}

	void WColorInspector::updateAlpha()
	{
		mColor->a(mA);
	}

}