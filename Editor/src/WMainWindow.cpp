#include "WMainWindow.h"

#include <QSettings>
#include <QWindow>
#include <QStatusBar>
#include <QProgressBar>
#include <QGridLayout >
#include <QtWidgets/QAction>
#include <QIcon>
#include <QRegExp>

#include <physic/Layer.h>

#include "Application.h"
#include <Logging.h>

#include "WGameContainer.h"

#include <QDialog>
#include <QFormLayout>
#include <QPushButton>
#include "inspector/WStringInspector.h"

#include "inspector/WInspectorComponent.h"

namespace maj::editor
{
	void WMainWindow::messageHandler(QtMsgType type, const QMessageLogContext& kContext, const QString& kMsg)
	{
		switch (type)
		{
			case QtInfoMsg:
			LOG(LogEditorInfo, kMsg.toStdString() << " (" << kContext.file << ":" << kContext.line << ")")
			break;

			case QtDebugMsg:
			LOG(LogEditorDebug, kMsg.toStdString() << " (" << kContext.file << ":" << kContext.line << ")")
			break;

			case QtWarningMsg:
			LOG(LogEditorWarning, kMsg.toStdString() << " (" << kContext.file << ":" << kContext.line << ")")
			break;

			case QtCriticalMsg:
			LOG(LogEditorCritical, kMsg.toStdString() << " (" << kContext.file << ":" << kContext.line << ")")
			break;

			case QtFatalMsg:
			LOG(LogEditorFatal, kMsg.toStdString() << " (" << kContext.file << ":" << kContext.line << ")")
			break;
		}
	}

	WMainWindow::WMainWindow(QWidget* parent, Qt::WindowFlags flags)
		: QMainWindow{ parent , flags }, mEngine{ nullptr }, mIsMaximizeOnPlay{false}
	{	
		setWindowState(windowState().setFlag(Qt::WindowMaximized, true));
		setDockNestingEnabled(true);

		qInstallMessageHandler(WMainWindow::messageHandler);

		std::string path { std::getenv("MAJ_ENGINE_PATH") };
		std::experimental::filesystem::current_path(path + "\\Game");
		mEngine = new Engine {};

		Service::get<resources::AssetsHandler>()->load(".");
		Service::get<scene::SceneHandler>()->load(".");

		setTabShape(QTabWidget::Triangular);

		mHierarchy = new WHierarchy { "Scene View", this };
		addDockWidget(Qt::LeftDockWidgetArea, mHierarchy);

		mInspector = new WInspector { "Inspector", this };
		addDockWidget(Qt::RightDockWidgetArea, mInspector);

		mAssetFolder = new WProject { "Asset_Folder", this };
		addDockWidget(Qt::LeftDockWidgetArea, mAssetFolder);

		mGameWidget = new WGameContainer { this };
	
		setCentralWidget(mGameWidget);

		Service::get<ScriptModule>()->load("Script.dll");

		win = new WMenuWin{ this };
		initToolBar();

		QSettings* settings{ new QSettings { "MAJ_Engine", "MAJ Engine", this } };
		settings->setValue("geometry", saveGeometry());
		settings->setValue("state", saveState());

		QTreeView* temp { mAssetFolder->treeView() };
		temp->connect(temp, &QTreeView::doubleClicked, mHierarchy, &WHierarchy::update);
		temp->connect(temp, &QTreeView::clicked, mInspector, &WInspector::updateAsset);

		QTreeWidget* temp2{ mHierarchy->treeWidget() };
		temp2->connect(temp2, &QTreeWidget::itemSelectionChanged, mInspector, &WInspector::selectionChanged);
		temp2->connect(temp2, &QTreeWidget::currentItemChanged, mInspector, &WInspector::updateScene);
		
		QStatusBar* status = new QStatusBar { this };
		setStatusBar(status);

		/*QProgressBar* Progress = new QProgressBar(status);
		Progress->setOrientation(Qt::Horizontal);
		Progress->setFixedSize(100, 20);
		status->addPermanentWidget(Progress);*/

		mEngine->mOnStopSimulation += [this]()
		{
			mInspector->clearToolBox();
		};

		connect(&mTime, &QTimer::timeout, this, &WMainWindow::update);
		mTime.start(0);
	}

	WMainWindow::~WMainWindow()
	{
		mTime.stop();

		if (mEngine)
			delete mEngine;
	}

	void WMainWindow::resetLayout()
	{
		QSettings settings("MAJ_Engine", "MAJ Engine");
		restoreGeometry(settings.value("geometry").toByteArray());
		restoreState(settings.value("state").toByteArray());
	}

	WHierarchy* WMainWindow::hierarchy()
	{
		return mHierarchy;
	}

	WInspector* WMainWindow::inspector()
	{
		return mInspector;
	}

	WProject* WMainWindow::assetFolder()
	{
		return mAssetFolder;
	}

	void WMainWindow::update()
	{
		mEngine->startFrame();
		mEngine->endFrame();
		Service::get<ww::Window>()->pollEvents();
	}

	void WMainWindow::saveCurrentScene()
	{
		scene::Scene* activeScene{ Service::get<scene::SceneHandler>()->activeScene() };
		if (activeScene)
		{
			activeScene->save((std::string{ activeScene->name() } +".scene").c_str());
		}
	}

	void WMainWindow::initToolBar()
	{
		mTool = addToolBar("ToolBar");
		mTool->setObjectName("ToolBar");
		mTool->setFloatable(false);
		mTool->setMovable(false);

		QWidget* separator{ new QWidget(mTool) };
		separator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
		mTool->addWidget(separator);

		mPlay = new QPushButton(mTool);
		mPlay->setShortcut(QKeySequence("F5"));
		mPlay->setIcon(QIcon("data/textures/play.png"));
		mPlay->setAutoFillBackground(true);
		mPlay->connect(mPlay, &QPushButton::pressed, this, &WMainWindow::clickOnPlay);
	
		mTool->addWidget(mPlay);
	
		mMaximizeOnPlay = new QPushButton(mTool);
		mMaximizeOnPlay->setAutoDefault(false);
		mMaximizeOnPlay->setDefault(false);
		mMaximizeOnPlay->setText(" Maximize On Play");
		mMaximizeOnPlay->setAutoFillBackground(true);
		mMaximizeOnPlay->connect(mMaximizeOnPlay, &QPushButton::pressed, this, &WMainWindow::MaxOnplay);
		mTool->addWidget(mMaximizeOnPlay);

		QWidget* separator2{ new QWidget(mTool) };
		separator2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
		mTool->addWidget(separator2);

	}
	void WMainWindow::MaxOnplay()
	{
		mIsMaximizeOnPlay = !mIsMaximizeOnPlay;
		if (mIsMaximizeOnPlay)
		{ 
			QPalette pal{ mMaximizeOnPlay->palette() };
			pal.setColor(QPalette::Button, QColor(42, 130, 218));
			mMaximizeOnPlay->setPalette(pal);
			mMaximizeOnPlay->update();
		}
		else
		{
			QPalette pal{ mMaximizeOnPlay->palette() };
			pal.setColor(QPalette::Button, QColor(53, 53, 53));
			mMaximizeOnPlay->setPalette(pal);
			mMaximizeOnPlay->update();
		}
	}

	void WMainWindow::clickOnPlay()
	{
		if (mEngine->isSimulating())
		{
			mPlay->setIcon(QIcon("data/textures/play.png"));
			mEngine->mOnStopSimulation();
			mHierarchy->setVisible(true);
			mInspector->setVisible(true);
			mAssetFolder->setVisible(true);

			mHierarchy->canRighClick();
			mInspector->canRighClick();
			mAssetFolder->canRighClick();

		}
		else if(Service::get<scene::SceneHandler>()->activeScene() && Service::get<scene::SceneHandler>()->activeScene()->cameras().count() >= 2u)
		{
			mPlay->setIcon(QIcon("data/textures/stop.png"));
			mEngine->mOnStartSimulation();
			if (mIsMaximizeOnPlay)
			{
				mHierarchy->setVisible(false);
				mInspector->setVisible(false);
				mAssetFolder->setVisible(false);
			}
			mHierarchy->canRighClick();
			mInspector->canRighClick();
			mAssetFolder->canRighClick();
		}
	}

	QDialog* WMainWindow::createDialog(std::function<void()> func)
	{
		QDialog* dial{ new QDialog() };
		dial->setWindowFlags(dial->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		dial->setWindowModality(Qt::ApplicationModal);
		QFormLayout* layout{ new QFormLayout(dial) };

		QPushButton* okButton{ new QPushButton("Confirm", dial) };
		okButton->setMaximumWidth(120);
		layout->addRow(okButton);
		layout->setAlignment(okButton, Qt::AlignCenter);

		QPushButton* cancelButton{ new QPushButton("Cancel", dial) };
		cancelButton->setMaximumWidth(120);
		layout->addRow(cancelButton);
		layout->setAlignment(cancelButton, Qt::AlignCenter);

		okButton->connect(okButton, &QPushButton::clicked, this, func);
		okButton->connect(okButton, &QPushButton::clicked, dial, &QDialog::done);
		cancelButton->connect(cancelButton, &QPushButton::clicked, dial, &QDialog::done);

		return dial;
	}

	void WMainWindow::createNameDialog(std::string& str, std::function<void()> func)
	{
		QDialog* dial{ createDialog(func) };

		WStringInspector* name{ new WStringInspector(str, dial) };
		QRegExp rx("[A-Za-z0-9]*");
		name->setValidator(new QRegExpValidator(rx, dial));
		static_cast<QFormLayout*>(dial->layout())->insertRow(0, "Name : ", name);

		dial->show();
	}

	void WMainWindow::createConfigDianlog()
	{
		QDialog* dial{ new QDialog() };
		dial->setWindowFlags(dial->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		dial->setWindowModality(Qt::ApplicationModal);

		QFormLayout* layout{ new QFormLayout(dial) };
		
		QTabWidget* tabWidget = new QTabWidget(dial);
		layout->addRow(tabWidget);
		tabWidget->setAutoFillBackground(true);

		if (mEngine)
		{
			QWidget* rootTr{ WInspectorComponent::registeredInspector()[mEngine->config().hash()](&mEngine->config(), tabWidget, nullptr) };
			tabWidget->addTab(rootTr, "Engine");
		}

		scene::SceneHandler* sceneHandler{ Service::get<scene::SceneHandler>() };
		if (sceneHandler)
		{
			QWidget* rootTr{ WInspectorComponent::registeredInspector()[sceneHandler->config().hash()](&sceneHandler->config(), tabWidget, nullptr) };
			tabWidget->addTab(rootTr, "Scene");
		}

		physic::LayerFilter& filter{ physic::Instance::layerFilter() };
		QWidget* rootfilter{ WInspectorComponent::registeredInspector()[filter.hash()](&filter, tabWidget, nullptr) };
		tabWidget->addTab(rootfilter, "Layer");

		dial->show();
	}


	bool WMainWindow::isMaximizedOnPlay()
	{
		return mIsMaximizeOnPlay;
	}

	bool WMainWindow::isRunning()
	{
		return mEngine->isSimulating();
	}
}