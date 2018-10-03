#include "WMenuWin.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QAction>
#include <QKeySequence>
#include <QSettings>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPicture>
#include <QPixmap>
#include <QDialog>

#include "WMainWindow.h"

namespace maj::editor
{
	WMenuWin::WMenuWin(WMainWindow* parent)
		:mParent{ parent }
	{
		createMenus();
	}

	WMenuWin::~WMenuWin()
	{}

	void WMenuWin::createMenus()
	{
		createFilesMenu();
		createAssetMenu();
		createGameObjectMenu();
		createWindowMenu();
		createHelpMenu();
	}

	void WMenuWin::createFilesMenu()
	{
		QMenu* menuFile = mParent->menuBar()->addMenu("&File");
		
		QAction* actionNewScene{ new QAction("&New Scene",menuFile) };
		actionNewScene->setShortcut(QKeySequence("Ctrl+N"));
		actionNewScene->connect(actionNewScene, &QAction::triggered, mParent->assetFolder(), &WProject::createScene);

		menuFile->addAction( actionNewScene);

		QAction* actionSaveScene{ new QAction("&Save Scene",menuFile) };
		actionSaveScene->setShortcut(QKeySequence("Ctrl+S"));
		menuFile->addAction( actionSaveScene);
		actionSaveScene->connect(actionSaveScene, &QAction::triggered, mParent, &WMainWindow::saveCurrentScene);

		menuFile->addSection("");

		QAction* actionProjectConfig{ new QAction("&Project Config",menuFile) };
		menuFile->addAction(actionProjectConfig);
		menuFile->addSection("");

		QAction* actionQuitter{ new QAction("&Quit",menuFile) };
		menuFile->addAction( actionQuitter);
		actionQuitter->setShortcut(QKeySequence("Alt+F4"));
		actionQuitter->setIcon(QIcon("C:/Users/m.garnier/Projet/MajMoteur/Moteur/data/textures/maj48x48.png"));

		QEventLoop::connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
		actionProjectConfig->connect(actionProjectConfig, &QAction::triggered, mParent, &WMainWindow::createConfigDianlog);
	}

	void WMenuWin::createAssetMenu()
	{
		QMenu* menuAssets{ mParent->menuBar()->addMenu("&Assets") };

		QAction* actionModel{ new QAction("&Model", menuAssets) };
		menuAssets->addAction( actionModel);

		QAction* actionSkeletalModel{ new QAction("&SkeletalModel", menuAssets) };
		menuAssets->addAction( actionSkeletalModel);
		menuAssets->addSection("");

		QAction* actionTexture{ new QAction("&Texture", menuAssets) };
		menuAssets->addAction( actionTexture);

		QAction* actionImage{ new QAction("&Image", menuAssets) };
		menuAssets->addAction( actionImage);

		QAction* actionFont{ new QAction("&Font", menuAssets) };
		menuAssets->addAction(actionFont);

		menuAssets->addSection("");

		QAction* actionMaterial{ new QAction("&Material", menuAssets) };
		menuAssets->addAction( actionMaterial);

		QAction* actionMaterialLayout{ new QAction("&Material Layout", menuAssets) };
		menuAssets->addAction( actionMaterialLayout);

		menuAssets->addSection("");

		QAction* actionPhysicMaterial{ new QAction("&Physic Material", menuAssets) };
		menuAssets->addAction(actionPhysicMaterial);

		actionModel->connect(actionModel, &QAction::triggered, mParent->assetFolder(), &WProject::createModel);
		actionSkeletalModel->connect(actionSkeletalModel, &QAction::triggered, mParent->assetFolder(), &WProject::createSkelModel);
		actionTexture->connect(actionTexture, &QAction::triggered, mParent->assetFolder(), &WProject::createTexture);
		actionImage->connect(actionImage, &QAction::triggered, mParent->assetFolder(), &WProject::createImage);
		actionFont->connect(actionFont, &QAction::triggered, mParent->assetFolder(), &WProject::createFont);
		actionMaterial->connect(actionMaterial, &QAction::triggered, mParent->assetFolder(), &WProject::createMaterial);
		actionMaterialLayout->connect(actionMaterialLayout, &QAction::triggered, mParent->assetFolder(), &WProject::createMatLayout);
		actionPhysicMaterial->connect(actionPhysicMaterial, &QAction::triggered, mParent->assetFolder(), &WProject::createPhysicMat);

	}

	void WMenuWin::createGameObjectMenu()
	{
		QMenu* menuGameObj{ mParent->menuBar()->addMenu("&GameObject") };

		QAction* actionEmptyAc{ new QAction("&Actor", menuGameObj) };
		menuGameObj->addAction( actionEmptyAc);

		QAction* actionAcCamera{ new QAction("&ACamera", menuGameObj) };
		menuGameObj->addAction( actionAcCamera);

		QAction* actionAcSky{ new QAction("&ASkybox", menuGameObj) };
		menuGameObj->addAction( actionAcSky);

		actionEmptyAc->connect(actionEmptyAc, &QAction::triggered, mParent->hierarchy(), &WHierarchy::addActor);
		actionAcCamera->connect(actionAcCamera, &QAction::triggered, mParent->hierarchy(), &WHierarchy::addACamera);
	}

	void WMenuWin::createWindowMenu()
	{
		QMenu* menuWindow{ mParent->menuBar()->addMenu("&Window") };

		QAction* actionInspector{ new QAction("&Inspector", menuWindow) };
		menuWindow->addAction(actionInspector);
		actionInspector->connect(actionInspector, &QAction::triggered, mParent, [this]()->void
		{
			if (!mParent->isMaximizedOnPlay() || !mParent->isRunning())
				mParent->inspector()->setVisible(true);
		});
		
		QAction* actionHierarchy{ new QAction("&Hierarchy", menuWindow) };
		menuWindow->addAction(actionHierarchy);

		actionHierarchy->connect(actionHierarchy, &QAction::triggered, mParent, [this]()->void
		{
			if (!mParent->isMaximizedOnPlay() || !mParent->isRunning())
				mParent->hierarchy()->setVisible(true);
		});

		QAction* actionAssetFolder{ new QAction("&Asset Folder", menuWindow) };
		menuWindow->addAction(actionAssetFolder);
		actionAssetFolder->connect(actionAssetFolder, &QAction::triggered, mParent, [this]()->void
		{
			if (!mParent->isMaximizedOnPlay() || !mParent->isRunning())
				mParent->assetFolder()->setVisible(true);
		});

		menuWindow->addSeparator();

		QAction* actionResetLayout{ new QAction("&Reset Layout", menuWindow) };
		actionResetLayout->setShortcut(QKeySequence("Ctrl+Shift+Y"));
		menuWindow->addAction(actionResetLayout);

		actionResetLayout->connect(actionResetLayout, &QAction::triggered, mParent, &WMainWindow::resetLayout);

	}

	void WMenuWin::createHelpMenu()
	{
		QMenu* menuHelp { mParent->menuBar()->addMenu("&Help") };
		QAction* actionCredits{ new QAction("&About ...", menuHelp) };
		actionCredits->setMenuRole(QAction::QuitRole);
		menuHelp->addAction(actionCredits);
		actionCredits->connect(actionCredits, &QAction::triggered, mParent, []()->void
		{
			QDialog* dial{ new QDialog() };
			dial->setWindowFlags(dial->windowFlags() & ~Qt::WindowContextHelpButtonHint);

			QVBoxLayout * VBoxLay{ new QVBoxLayout(dial) };
			QLabel* Img = new QLabel(dial);
			QPixmap pixmap("data/textures/Credit.png");
			Img->setPixmap(pixmap);
			VBoxLay->addWidget(Img);
			QLabel* Version = new QLabel("Version 0.7.1 Personal", dial);
			VBoxLay->addWidget(Version);
			QLabel* Dev = new QLabel("Martin Garnier, Aurel Pierre, Jannah Mekhaemar", dial);
			VBoxLay->addWidget(Dev);

			QHBoxLayout * HBoxLay{ new QHBoxLayout() };
			QLabel* ImgVulk = new QLabel(dial);
			QPixmap pixmapV("data/textures/Vulkan.png");
			ImgVulk->setPixmap(pixmapV);
			HBoxLay->addWidget(ImgVulk);

			QLabel* ImgP = new QLabel(dial);
			QPixmap pixmapP("data/textures/PhysX.png");
			ImgP->setPixmap(pixmapP);
			HBoxLay->addWidget(ImgP);

			VBoxLay->addLayout(HBoxLay);

			dial->show();
		});
	}
}