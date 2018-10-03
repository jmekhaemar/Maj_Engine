#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <QtWidgets/QApplication>
#include "WMainWindow.h"

namespace maj::editor
{
	class Application : public QApplication
	{
	public:
		Application(int argc, char** argv);
		~Application();
		WMainWindow* mainWindow();

	private:
		WMainWindow* mMainW;
	};
}

#endif