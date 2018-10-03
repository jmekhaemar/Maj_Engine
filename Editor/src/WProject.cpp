#include "WProject.h"

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QAbstractItemView>
#include <QtWidgets/QMenu>

#include <QMouseEvent>
#include <QApplication>
#include <QMimeData>
#include <QDrag>

#include <experimental/filesystem>

#include "WHierarchy.h"
#include "WMainWindow.h"

#include "scene/CPointLight.h"
#include "scene/SceneHandler.h"
#include "scene/ACamera.h"
#include "scene/ASkybox.h"

#include "Asset.h"
#include <MaterialLayout.h>
#include <Material.h>
#include <physic/PhysicMaterial.h>
#include <Image.h>
#include <Texture.h>
#include <resources/Model.h>
#include <resources/SkeletalModel.h>

namespace maj::editor
{
	WProject::WTreeView::WTreeView(WProject* parent)
		:QTreeView(parent), mParent{ parent }
	{
		qApp->installEventFilter(this);
	}

	WProject::WTreeView::~WTreeView()
	{}

	bool WProject::WTreeView::eventFilter(QObject* obj, QEvent* event)
	{
		if (obj == this && event->type() == QEvent::KeyPress)
		{
			QKeyEvent* key = static_cast<QKeyEvent*>(event);
			if (key->key() == Qt::Key_Delete && !mParent->mParent->isRunning())
			{
				//Delete was pressed
				QModelIndex index = selectionModel()->currentIndex();
				if (index != QModelIndex())
				{
					removeScene();
					return true;
				}
			}
		}
		return false;
	}

	void WProject::WTreeView::removeScene()
	{
		const QFileSystemModel* kFileSystem { mParent->fileSystem() };
		std::string selectedName { QDir::current().relativeFilePath(kFileSystem->filePath(selectionModel()->currentIndex())).toStdString() };
		std::experimental::filesystem::path path { selectedName };

		scene::SceneHandler* sceneHandler { Service::get<scene::SceneHandler>() };
		scene::Scene* activeScene { sceneHandler->activeScene() };

		if (path.extension().generic_string() == ".asset" || std::experimental::filesystem::is_directory(path))
			return;

		std::string sceneName { scene::SceneHandler::sceneFromPath(path) };
		if (activeScene && sceneName == activeScene->name())
			return;

		sceneHandler->removeScene(path);
		mParent->mDirModel->remove(selectionModel()->currentIndex());
	}

	WProject::WProject(const QString& kTitle, WMainWindow* parent, Qt::WindowFlags flags)
		: QDockWidget{ kTitle , parent,  flags }, mParent{ parent }
	{
		setObjectName(kTitle);

		setAutoFillBackground(true);
		mTreeView = new WTreeView(this);
		mDirModel = new QFileSystemModel(mTreeView);

		mDirModel->setReadOnly(false);
		mTreeView->setEditTriggers(QAbstractItemView::SelectedClicked | QAbstractItemView::EditKeyPressed);
		mTreeView->setDefaultDropAction(Qt::CopyAction);//Qt::IgnoreAction, Qt::MoveAction, Qt::CopyAction, Qt::LinkAction, Qt::LinkAction

		mTreeView->setModel(mDirModel);

		mDirModel->setNameFilters(QStringList() << "*.asset" << "*.scene");
		mDirModel->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
		mDirModel->setNameFilterDisables(false);
		mDirModel->setRootPath(QDir::currentPath());

		std::string str{ mDirModel->rootPath().toStdString() + "/asset/" };
		mTreeView->setRootIndex(mDirModel->index({ str.c_str() }));
		
		mTreeView->setHeaderHidden(true);
		mTreeView->setExpandsOnDoubleClick(true);
		mTreeView->setRootIsDecorated(true);
		mTreeView->setSortingEnabled(false);
		mTreeView->setDragDropMode(QAbstractItemView::DragDrop);

		for (uint8_t i{ 1u } ; i < 4u; i++ )
			mTreeView->setColumnHidden(i, true);
		
		mTreeView->setDropIndicatorShown(true);


		mTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
		canRighClick();
	//	mTreeView->connect(mTreeView, &QTreeView::customContextMenuRequested, this, &WProject::showContextMenu);

		mDirModel->connect(mDirModel, &QFileSystemModel::fileRenamed, this, &WProject::renameScene);

		setWidget(mTreeView);
	}


	WProject::~WProject()
	{}

	const QFileSystemModel* WProject::fileSystem() const
	{
		return mDirModel;
	}

	QTreeView* WProject::treeView()
	{
		return mTreeView;
	}

	void WProject::showContextMenu(const QPoint& pos)
	{
		mCurrentModelIndex = pos;

		QMenu contextMenu;
		QAction* addActorS{ contextMenu.addAction("New Scene") };

		QAction* removeScenes{ contextMenu.addAction("Remove Scene") };

		QMenu* menuAssets{ new QMenu{ "Add Asset" , &contextMenu } };
		contextMenu.addMenu(menuAssets);

		QAction* mActionModel{ new QAction("&Model", menuAssets) };
		menuAssets->addAction(mActionModel);
		
		QAction* mActionSkeletalModel{ new QAction("&SkeletalModel", menuAssets) };
		menuAssets->addAction(mActionSkeletalModel);
		
		menuAssets->addSection("");
		
		QAction* mActionTexture{ new QAction("&Texture", menuAssets) };
		menuAssets->addAction(mActionTexture);
		
		QAction* mActionImage{ new QAction("&Image", menuAssets) };
		menuAssets->addAction(mActionImage);
		
		QAction* mActionFont{ new QAction("&Font", menuAssets) };
		menuAssets->addAction(mActionFont);
		
		menuAssets->addSection("");
		
		QAction* mActionMaterial{ new QAction("&Material", menuAssets) };
		menuAssets->addAction(mActionMaterial);
		
		QAction* mActionMaterialLayout{ new QAction("&Material Layout", menuAssets) };
		menuAssets->addAction(mActionMaterialLayout);
		
		menuAssets->addSection("");
		
		QAction* mActionPhysicMaterial{ new QAction("&Physic Material", menuAssets) };
		menuAssets->addAction(mActionPhysicMaterial);
		
		addActorS->connect(addActorS, &QAction::triggered, this, &WProject::createScene);
		removeScenes->connect(removeScenes, &QAction::triggered, mTreeView, &WProject::WTreeView::removeScene);

		mActionModel->connect(mActionModel, &QAction::triggered, this, &WProject::createModel);
		mActionSkeletalModel->connect(mActionSkeletalModel, &QAction::triggered, this, &WProject::createSkelModel);
		mActionTexture->connect(mActionTexture, &QAction::triggered, this, &WProject::createTexture);
		mActionImage->connect(mActionImage, &QAction::triggered, this, &WProject::createImage);
		mActionFont->connect(mActionFont, &QAction::triggered, this, &WProject::createFont);
		mActionMaterial->connect(mActionMaterial, &QAction::triggered, this, &WProject::createMaterial);
		mActionMaterialLayout->connect(mActionMaterialLayout, &QAction::triggered, this, &WProject::createMatLayout);
		mActionPhysicMaterial->connect(mActionPhysicMaterial, &QAction::triggered, this, &WProject::createPhysicMat);

		contextMenu.exec(mapToGlobal(pos));
	}

	void WProject::createScene()
	{
		const QModelIndex& kModel { mTreeView->selectionModel()->currentIndex() };
		QFileInfo file { mDirModel->fileInfo(kModel) };
		std::string selectedName { QDir::current().relativeFilePath(file.isDir() ? file.filePath() : file.dir().path()).toStdString() + "/"};

		mParent->createNameDialog(mFuturName, [this, selectedName] ()->void
								  {
									  if (mFuturName == "" || mFuturName == " ")
										  mFuturName = "Default_Scene";

									  std::string path { selectedName + mFuturName + ".scene" };

									  scene::SceneHandler* sceneHandler { Service::get<scene::SceneHandler>() };
									  sceneHandler->addScene(path.c_str());

									  scene::Scene* newScene { sceneHandler->scene((selectedName + mFuturName).c_str()) };
									  if (newScene)
										  newScene->save(path.c_str());
								  });
	}

	void WProject::renameScene(const QString& kPath, const QString& kOldName, const QString& kNewName)
	{
		std::string path { QDir::current().relativeFilePath(kPath).toStdString() };
		std::string oldName { scene::SceneHandler::sceneFromPath(path + "/" + kOldName.toStdString()) };
		std::string newName { scene::SceneHandler::sceneFromPath(path + "/" + kNewName.toStdString()) };

		scene::SceneHandler* sceneHandler { Service::get<scene::SceneHandler>() };
		scene::Scene* scene { sceneHandler->scene(oldName.c_str()) };
		scene::Scene* newScene { sceneHandler->scene(newName.c_str()) };

		if (!newScene)
		{ 
			sceneHandler->rename(oldName.c_str(), newName.c_str());
			if(scene == sceneHandler->activeScene())
				mDirModel->connect(mDirModel, &QFileSystemModel::fileRenamed, static_cast<WMainWindow*>(mParent)->hierarchy(), &WHierarchy::rename);
		}
	}

	void WProject::createModel()
	{
		const QModelIndex& kModel { mTreeView->selectionModel()->currentIndex() };
		QFileInfo file { mDirModel->fileInfo(kModel) };
		std::string selectedName { QDir::current().relativeFilePath(file.isDir() ? file.filePath() : file.dir().path()).toStdString() + "/" };

		mParent->createNameDialog(mFuturName, [this, selectedName] ()->void
								  {
									  if (mFuturName == "" || mFuturName == " ")
										  mFuturName = "New_Model";

									  resources::Model* newAsset { Asset<resources::Model>::add(selectedName + mFuturName, "data/models/cube.obj") };
									  newAsset->save((selectedName + mFuturName + ".asset").c_str());
								  });
	}

	void WProject::createSkelModel()
	{
		const QModelIndex& kModel { mTreeView->selectionModel()->currentIndex() };
		QFileInfo file { mDirModel->fileInfo(kModel) };
		std::string selectedName { QDir::current().relativeFilePath(file.isDir() ? file.filePath() : file.dir().path()).toStdString() + "/" };

		mParent->createNameDialog(mFuturName, [this, selectedName] ()->void
								  {
									  if (mFuturName == "" || mFuturName == " ")
										  mFuturName = "New_Skeletal_Model";

									  resources::SkeletalModel* newAsset { Asset<resources::SkeletalModel>::add(selectedName + mFuturName, "data/models/cube.fbx") };
									  newAsset->save((selectedName + mFuturName + ".asset").c_str());
								  });
	}

	void WProject::createTexture() 
	{
		const QModelIndex& kModel { mTreeView->selectionModel()->currentIndex() };
		QFileInfo file { mDirModel->fileInfo(kModel) };
		std::string selectedName { QDir::current().relativeFilePath(file.isDir() ? file.filePath() : file.dir().path()).toStdString() + "/" };

		mParent->createNameDialog(mFuturName, [this, selectedName] ()->void
								  {
									  if (mFuturName == "" || mFuturName == " ")
										  mFuturName = "New_Texture";

									  rhi::Texture* newAsset { Asset<rhi::Texture>::add(selectedName + mFuturName, Asset<rhi::Image>::get("asset/default/default_Img")) };
									  newAsset->save((selectedName + mFuturName + ".asset").c_str());
								  });
	}

	void WProject::createImage()
	{
		const QModelIndex& kModel { mTreeView->selectionModel()->currentIndex() };
		QFileInfo file { mDirModel->fileInfo(kModel) };
		std::string selectedName { QDir::current().relativeFilePath(file.isDir() ? file.filePath() : file.dir().path()).toStdString() + "/" };

		mParent->createNameDialog(mFuturName, [this, selectedName] ()->void
								  {
									  if (mFuturName == "" || mFuturName == " ")
										  mFuturName = "New_Image";

									  rhi::Format form { rhi::Format::EComponent::R8G8B8A8, rhi::Format::EType::UNORM };
									  rhi::Image* newAsset { Asset<rhi::Image>::add(selectedName + mFuturName, "data/textures/default_img.png", form) };
									  newAsset->save((selectedName + mFuturName + ".asset").c_str());
								  });
	}

	void WProject::createFont() 
	{
		const QModelIndex& kModel { mTreeView->selectionModel()->currentIndex() };
		QFileInfo file { mDirModel->fileInfo(kModel) };
		std::string selectedName { QDir::current().relativeFilePath(file.isDir() ? file.filePath() : file.dir().path()).toStdString() + "/" };

		mParent->createNameDialog(mFuturName, [this, selectedName] ()->void
								  {
									  if (mFuturName == "" || mFuturName == " ")
										  mFuturName = "New_Font";

									  gui::Font* newAsset { Asset<gui::Font>::add(selectedName + mFuturName) };
									  newAsset->save((selectedName + mFuturName + ".asset").c_str());
								  });
	}

	void WProject::createMaterial()
	{
		const QModelIndex& kModel { mTreeView->selectionModel()->currentIndex() };
		QFileInfo file { mDirModel->fileInfo(kModel) };
		std::string selectedName { QDir::current().relativeFilePath(file.isDir() ? file.filePath() : file.dir().path()).toStdString() + "/" };

		mParent->createNameDialog(mFuturName, [this, selectedName] ()->void
								  {
									  if (mFuturName == "" || mFuturName == " ")
										  mFuturName = "New_Material";

									  rhi::Material* tempMat { Asset<rhi::Material>::get("asset/default/objMaterial") };
									  rhi::Material::Info tempInfo { tempMat->info(), Service::get<rhi::Renderer>()->swapChain(0u).renderPass(),tempMat->info().mSubpassIndex };
									  rhi::Material* newAsset { Asset<rhi::Material>::add(selectedName + mFuturName,*tempMat, std::move(tempInfo)) };
									  newAsset->save((selectedName + mFuturName + ".asset").c_str());
								  });
	}

	void WProject::createMatLayout()
	{
		const QModelIndex& kModel { mTreeView->selectionModel()->currentIndex() };
		QFileInfo file { mDirModel->fileInfo(kModel) };
		std::string selectedName { QDir::current().relativeFilePath(file.isDir() ? file.filePath() : file.dir().path()).toStdString() + "/" };

		mParent->createNameDialog(mFuturName, [this, selectedName] ()->void
								  {
									  if (mFuturName == "" || mFuturName == " ")
										  mFuturName = "New_Mat_Layout";

									  rhi::MaterialLayout* newAsset { Asset<rhi::MaterialLayout>::add(selectedName + mFuturName, std::map<uint32_t, maj::rhi::LayoutSet>{ }) };
									  newAsset->save((selectedName + mFuturName + ".asset").c_str());
								  });
	}

	void WProject::createPhysicMat()
	{
		const QModelIndex& kModel { mTreeView->selectionModel()->currentIndex() };
		QFileInfo file { mDirModel->fileInfo(kModel) };
		std::string selectedName { QDir::current().relativeFilePath(file.isDir() ? file.filePath() : file.dir().path()).toStdString() + "/" };

		mParent->createNameDialog(mFuturName, [this, selectedName] ()->void
								  {
									  if (mFuturName == "" || mFuturName == " ")
										  mFuturName = "New_Physical_Mat";

									  physic::PhysicMaterial* newAsset { Asset<physic::PhysicMaterial >::add(selectedName + mFuturName) };
									  newAsset->save((selectedName + mFuturName + ".asset").c_str());
								  });
	}

	void WProject::canRighClick()
	{
		if (!mParent->isRunning())
			mTreeView->connect(mTreeView, &QTreeView::customContextMenuRequested, this, &WProject::showContextMenu);
		else
			mTreeView->disconnect(mTreeView, &QTreeView::customContextMenuRequested, this, &WProject::showContextMenu);
	}
}