#include "WHierarchy.h"

#include <experimental/filesystem>

#include <QMouseEvent>
#include <QApplication>

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QAbstractItemView>
#include <QtWidgets/QMenu>
#include <QStringList>
#include <QMouseEvent>

#include <scene/SceneHandler.h>
#include <scene/AGameCamera.h>
#include <scene/ASkybox.h>

#include "WMainWindow.h"

namespace maj::editor
{
	WHierarchy::WTreeWidget::WTreeWidget(WHierarchy * parent)
		:QTreeWidget{ parent }, mParent{ parent }
	{
		qApp->installEventFilter(this);
		setAcceptDrops(false);
	}

	WHierarchy::WTreeWidget::~WTreeWidget()
	{}

	void WHierarchy::WTreeWidget::updateSelection()
	{
		gizmo::GizmoHandler* gizmoHandler { Service::get<gizmo::GizmoHandler>() };
		scene::Actor* selected { gizmoHandler->selectedActor() };
		if (!selected)
		{
			selectionModel()->clear();
			return;
		}

		const std::string& kName { selected->name() };
		QList<QTreeWidgetItem*> foundItems { findItems(QString::fromStdString(kName), Qt::MatchExactly | Qt::MatchRecursive) };
		if (foundItems.count() < 1u)
		{
			selectionModel()->clear();
			return;
		}

		setCurrentItem(foundItems[0u]);
	}

	void WHierarchy::WTreeWidget::mousePressEvent(QMouseEvent* event)
	{
		QModelIndex item { indexAt(event->pos()) };

		if (!item.isValid())
			selectionModel()->clear();
		else
			setCurrentIndex(item);

		QTreeWidget::mousePressEvent(event);
	}

	bool  WHierarchy::WTreeWidget::eventFilter(QObject* obj, QEvent* event)
	{
		if (obj == this && event->type() == QEvent::KeyPress)
		{
			QKeyEvent* key = static_cast<QKeyEvent*>(event);
			if (key->key() == Qt::Key_Delete && !mParent->mParent->isRunning())
			{
				QModelIndex index = selectionModel()->currentIndex();
				if (index != QModelIndex())
				{
					removeActor();
					return true;
				}
			}
		}
		return false;
	}

	void WHierarchy::WTreeWidget::removeActor()
	{
		QTreeWidgetItem* item{ currentItem() };
		if (!item)
			return;

		scene::SceneHandler* sceneHandler{ Service::get<scene::SceneHandler>() };
		scene::Scene* scene{ sceneHandler->activeScene() };
		if (!scene)
			return;

		std::string name{ item->data(currentColumn(), Qt::DisplayRole).toString().toStdString() };
		scene::Actor* actor{ scene->actor(name) };
		scene->removeActor(actor);

		QTreeWidgetItem* parent{ item->parent() };
		if (parent)
			delete parent->takeChild(parent->indexOfChild(item));
		else
			delete takeTopLevelItem(indexOfTopLevelItem(item));
	}



	WHierarchy::WHierarchy(const QString& kTitle, WMainWindow* parent, Qt::WindowFlags flags)
		: QDockWidget { kTitle, parent, flags }, mParent { parent }
	{
		setObjectName(kTitle);
		setAcceptDrops(false);

		setAutoFillBackground(true);
		mTree = new WTreeWidget(this);

		load(Service::get<scene::SceneHandler>()->activeScene());
	
		mTree->setContextMenuPolicy(Qt::CustomContextMenu);
		canRighClick();
		//connect(mTree, &QTreeWidget::customContextMenuRequested, this, &WHierarchy::showContextMenu);
		
		setWidget(mTree);

		Service::get<gizmo::GizmoHandler>()->mOnSelectionChanged += std::bind(&WTreeWidget::updateSelection, mTree);
	}

	WHierarchy::~WHierarchy()
	{}

	void WHierarchy::update(const QModelIndex& kModel)
	{
		const QFileSystemModel* kFileSystem { mParent->assetFolder()->fileSystem() };
		std::string str { QDir::current().relativeFilePath(kFileSystem->filePath(kModel)).toStdString() };
		std::experimental::filesystem::path path { str };

		if (path.extension().generic_string() == ".asset" || std::experimental::filesystem::is_directory(path))
			return;

		std::string fileName { scene::SceneHandler::sceneFromPath(path) };

		scene::SceneHandler* sceneHandler{ Service::get<scene::SceneHandler>() };
		scene::Scene* activeScene { sceneHandler->activeScene() };

		if (activeScene && activeScene->name() == fileName)
			return;
		else
			mTree->clear();
		
		if (activeScene)
			activeScene->save((activeScene->name() + ".scene").c_str());
		
		sceneHandler->activeScene(fileName.c_str());
		load(sceneHandler->activeScene());
	}

	void WHierarchy::load(scene::Scene* activeScene)
	{
		if (!activeScene)
			return;

		mTree->setHeaderLabel(activeScene->name().c_str());
		browse(meta::Field<scene::Scene, 1u>::get(activeScene));
	}

	void WHierarchy::showContextMenu(const QPoint& kPos)
	{
		QMenu contextMenu {};
		QMenu addActor { "Add Actor" , &contextMenu };
		contextMenu.addMenu(&addActor);
		QAction* addActorS = addActor.addAction("Actor");
		QAction* addACamera = addActor.addAction("ACamera");
		QAction* addAskyBox = addActor.addAction("ASkybox");
		contextMenu.addSeparator();
		QAction* remove { contextMenu.addAction("Remove Actor") };
				
		addActorS->connect(addActorS, &QAction::triggered, this, &WHierarchy::addActor);
		addACamera->connect(addACamera, &QAction::triggered, this, &WHierarchy::addACamera);
		addAskyBox->connect(addAskyBox, &QAction::triggered, this, &WHierarchy::addASkybox);
		remove->connect(remove, &QAction::triggered, mTree, &WHierarchy::WTreeWidget::removeActor);

		contextMenu.exec(mapToGlobal(kPos));
	}

	void WHierarchy::addActor()
	{
		mParent->createNameDialog(mFuturName, [this] ()->void
								  {
									  QTreeWidgetItem* currentItem { mTree->currentItem() };
									  bool hasParent { currentItem != nullptr };
									  
									  scene::SceneHandler* sceneHandler { Service::get<scene::SceneHandler>() };
									  scene::Scene* scene { sceneHandler->activeScene() };
									  if (!scene)
										  return;

									  if (mFuturName == "" || mFuturName == " ")
										  mFuturName = "Default_Actor";

									  if (hasParent)
									  {
										  std::string parentName { currentItem->data(mTree->currentColumn(), Qt::DisplayRole).toString().toStdString() };
										  scene::Actor* parentActor { scene->actor(parentName) };
										  if (!parentActor)
											  return;

										  parentActor->addChild<scene::Actor>(mFuturName.c_str());
									  }
									  else
										  scene->addActor<scene::Actor>(mFuturName.c_str());

									  QTreeWidgetItem* child { nullptr };
									  QStringList name { QString::fromStdString(mFuturName) };

									  if (!currentItem)
										  child = new QTreeWidgetItem(mTree, name, QTreeWidgetItem::UserType);
									  else
										  child = new QTreeWidgetItem(currentItem, name, QTreeWidgetItem::UserType);
								  });
	}

	void WHierarchy::addACamera()
	{
		mParent->createNameDialog(mFuturName, [this] ()->void
								  {
									  QTreeWidgetItem* currentItem { mTree->currentItem() };
									  bool hasParent { currentItem != nullptr };

									  scene::SceneHandler* sceneHandler { Service::get<scene::SceneHandler>() };
									  scene::Scene* scene { sceneHandler->activeScene() };
									  if (!scene)
										  return;

									  if (mFuturName == "" || mFuturName == " ")
										  mFuturName = "Default_Camera";

									  if (hasParent)
									  {
										  std::string parentName { currentItem->data(mTree->currentColumn(), Qt::DisplayRole).toString().toStdString() };
										  scene::Actor* parentActor { scene->actor(parentName) };
										  if (!parentActor)
											  return;

										  parentActor->addChild<scene::AGameCamera>(mFuturName.c_str());
									  }
									  else
										  scene->addActor<scene::AGameCamera>(mFuturName.c_str());

									  QTreeWidgetItem* child { nullptr };
									  QStringList name { QString::fromStdString(mFuturName) };

									  if (!currentItem)
										  child = new QTreeWidgetItem(mTree, name, QTreeWidgetItem::UserType);
									  else
										  child = new QTreeWidgetItem(currentItem, name, QTreeWidgetItem::UserType);
								  });
	}

	void WHierarchy::addASkybox()
	{
		mParent->createNameDialog(mFuturName, [this] ()->void
								  {
									  QTreeWidgetItem* currentItem { mTree->currentItem() };
									  bool hasParent { currentItem != nullptr };

									  scene::SceneHandler* sceneHandler { Service::get<scene::SceneHandler>() };
									  scene::Scene* scene { sceneHandler->activeScene() };
									  if (!scene)
										  return;

									  if (mFuturName == "" || mFuturName == " ")
										  mFuturName = "Default_Actor";

									  if (hasParent)
									  {
										  std::string parentName { currentItem->data(mTree->currentColumn(), Qt::DisplayRole).toString().toStdString() };
										  scene::Actor* parentActor { scene->actor(parentName) };
										  if (!parentActor)
											  return;

										  parentActor->addChild<scene::ASkybox>(mFuturName.c_str(), Asset<rhi::Material>::get("asset/default/skyboxMaterial"));
									  }
									  else
										  scene->addActor<scene::ASkybox>(mFuturName.c_str(), Asset<rhi::Material>::get("asset/default/skyboxMaterial"));

									  QTreeWidgetItem* child { nullptr };
									  QStringList name { QString::fromStdString(mFuturName) };

									  if (!currentItem)
										  child = new QTreeWidgetItem(mTree, name, QTreeWidgetItem::UserType);
									  else
										  child = new QTreeWidgetItem(currentItem, name, QTreeWidgetItem::UserType);
								  });
	}

	void WHierarchy::rename(const QString& kPath, const QString& kOldName, const QString& kNewName)
	{
		(void) kPath;
		(void) kOldName;

		std::string strNewName { kNewName.toStdString() };
		std::experimental::filesystem::path newPath { strNewName };
		std::string newSceneName { newPath.stem().generic_string() };

		mTree->setHeaderLabel(newSceneName.c_str());
	}

	QTreeWidget* WHierarchy::treeWidget()
	{
		return mTree;
	}

	void WHierarchy::browse(const scene::Actor* kActor, QTreeWidgetItem* parentWidget)
	{
		if (!kActor)
			return;

		uint8_t childCount { kActor->childCount() };
		for (uint8_t index { 0u } ; index < childCount ; ++index)
		{
			const scene::Actor* kChild { kActor->child(index) };
			QStringList name { QString::fromStdString(kChild->name()) };
			QTreeWidgetItem* childWidget { nullptr };

			if (!parentWidget)
				childWidget = new QTreeWidgetItem { mTree, name, QTreeWidgetItem::UserType };
			else
				childWidget = new QTreeWidgetItem { parentWidget, name, QTreeWidgetItem::UserType };

			browse(kChild, childWidget);
		}
	}

	void WHierarchy::canRighClick()
	{
		if (!mParent->isRunning())
			mTree->connect(mTree, &QTreeWidget::customContextMenuRequested, this, &WHierarchy::showContextMenu);
		else
			mTree->disconnect(mTree, &QTreeWidget::customContextMenuRequested, this, &WHierarchy::showContextMenu);
	}
}