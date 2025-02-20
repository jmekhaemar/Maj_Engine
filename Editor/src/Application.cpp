#include "Application.h"

#include <QIcon>
#include <QPixmap>
#include <QSplashScreen>
#include <QStyleFactory>
#include "CustomStyle.h"

namespace maj::editor
{
	Application::Application(int argc, char** argv)
		: QApplication{ argc, argv }
	{
		setStyle(new CustomStyle);

		QPalette palette;
		palette.setColor(QPalette::Window, QColor(53, 53, 53));
		palette.setColor(QPalette::WindowText, Qt::white);
		palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
		palette.setColor(QPalette::Base, QColor(42, 42, 42));
		palette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
		palette.setColor(QPalette::ToolTipBase, Qt::white);
		palette.setColor(QPalette::ToolTipText, QColor(53, 53, 53));
		palette.setColor(QPalette::Text, Qt::white);
		palette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
		palette.setColor(QPalette::Dark, QColor(35, 35, 35));
		palette.setColor(QPalette::Shadow, QColor(20, 20, 20));
		palette.setColor(QPalette::Button, QColor(53, 53, 53));
		palette.setColor(QPalette::ButtonText, Qt::white);
		palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));
		palette.setColor(QPalette::BrightText, Qt::red);
		palette.setColor(QPalette::Link, QColor(42, 130, 218));
		palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
		palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80, 80, 80));
		palette.setColor(QPalette::HighlightedText, Qt::white);
		palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(127, 127, 127));
		setPalette(palette);

		QLocale::setDefault(QLocale("us"));
		
		QPixmap pixmap("data/textures/LogoMAJWhite.png");
		QSplashScreen splash(pixmap);
		splash.show();
		processEvents();
		setWindowIcon(QIcon("data/textures/maj48x48.png"));
		setApplicationName("MAJ Engine");

		mMainW = new WMainWindow();
		splash.finish(mMainW);
		mMainW->show();
	}

	Application::~Application()
	{
		if(mMainW)
			delete mMainW;
	}

	WMainWindow* Application::mainWindow()
	{
		return mMainW;
	}
}