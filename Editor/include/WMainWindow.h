#ifndef _WMAIN_WINDOW_H_
#define _WMAIN_WINDOW_H_

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include <QtWidgets/QToolBar>
#include <QPushButton>

#include <Engine.h>
#include <Logging.h>

#include "inspector/WInspector.h"
#include "WProject.h"
#include "WMenuWin.h"
#include "WHierarchy.h"

namespace maj::editor
{
	class WMainWindow : public QMainWindow
	{
	private:
		static void messageHandler(QtMsgType type, const QMessageLogContext& kContext, const QString& kMsg);

	public:
		WMainWindow(QWidget* parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());
		~WMainWindow();

		void resetLayout();
		void saveCurrentScene();

		WHierarchy* hierarchy();
		WInspector* inspector();
		WProject* assetFolder();

		void clickOnPlay();
		void MaxOnplay();

		QDialog* createDialog(std::function<void()> func = nullptr);
		void createNameDialog(std::string& str, std::function<void()> func = nullptr);
		void createConfigDianlog();
		bool isMaximizedOnPlay();
		bool isRunning();

	private:

		void update();
		void initToolBar();

		Engine* mEngine;
		WMenuWin* win;

		WHierarchy* mHierarchy;
		WInspector* mInspector;
		WProject* mAssetFolder;
		
		QToolBar* mTool;
		QPushButton* mPlay;
		QPushButton* mMaximizeOnPlay;
		bool mIsMaximizeOnPlay;
		
		QWidget* mGameWidget;
		
		QTimer mTime;
	};
}

LOGGING_CREATE_LEVEL(EditorInfo, maj::log::Severity::INFO, "[ INFO ][ EDITOR ] ")
LOGGING_CREATE_LEVEL(EditorDebug, maj::log::Severity::DEBUG, "[ DEBUG ][ EDITOR ] ")
LOGGING_CREATE_LEVEL(EditorWarning, maj::log::Severity::WARNING, "[ WARNING ][ EDITOR ] ")
LOGGING_CREATE_LEVEL(EditorCritical, maj::log::Severity::ERR, "[ CRITICAL ][ EDITOR ] ")
LOGGING_CREATE_LEVEL(EditorFatal, maj::log::Severity::ERR, "[ FATAL ][ EDITOR ] ")

#endif