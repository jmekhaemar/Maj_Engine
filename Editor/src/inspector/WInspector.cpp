#include "inspector/WInspector.h"

#include <experimental/filesystem>

#include <QtWidgets/QAbstractItemView>
#include <QtWidgets/QMenu>
#include <QFileSystemModel>
#include <QStringList>
#include <QWindow>

#include <Logging.h>
#include <ScriptModule.h>
#include <scene/SceneHandler.h>

#include "inspector/WInspectorAsset.h"
#include "inspector/RegisteredInspector.h"

#include "WMainWindow.h"

namespace maj::editor
{
	WInspector::WInspector(const QString& kTitle, WMainWindow* parent, Qt::WindowFlags flags)
		: QDockWidget{ kTitle , parent,  flags }, mParent{ parent }, mCurrentActor { nullptr }, mTransfIndex{ -1 }
	{
		setObjectName(kTitle);
		mTool = new QToolBox(this);

		mTool->setContextMenuPolicy(Qt::CustomContextMenu);
		canRighClick();
		//connect(mTool, &QToolBox::customContextMenuRequested, this, &WInspector::showContextMenu);
		
		setWidget(mTool);

		// Inspector registration
		ScriptModule* script { Service::get<ScriptModule>() };
		script->mOnLoaded += [script] () -> void
		{
			typedef std::unordered_map<uint32_t, std::function<WInspectorComponent*(ISerializable*, QWidget*, QFormLayout*)>> InspectorMap;
			ScriptModule::Function<const InspectorMap&> inspectorFunc { script->function<const InspectorMap&>("registeredInspector") };
			if (inspectorFunc)
				WInspectorComponent::registerInspector(inspectorFunc());

			typedef std::unordered_map<uint32_t, WInspectorComponent::ComponentInfo> ComponentMap;
			ScriptModule::Function<const ComponentMap&> componentFunc { script->function<const ComponentMap&>("registeredComponents") };
			if (componentFunc)
				WInspectorComponent::registerComponents(componentFunc());
		};

		script->mOnUnload += [] () -> void
		{
			WInspectorComponent::registeredInspector().clear();
			WInspectorComponent::registeredComponents().clear();
		};
	}

	WInspector::WInspector(QWidget* parent, Qt::WindowFlags flags)
		:QDockWidget{ parent,  flags }
	{
		setMinimumSize(200, 300);
		setMaximumSize(300, 700);
		mTool = new QToolBox();
		setWidget(mTool);
	}

	void WInspector::updateAsset(const QModelIndex& kModel)
	{
		std::string name { kModel.data().toString().toStdString() };
		std::experimental::filesystem::path p { name };

		//LOG(LogInfo, p.generic_string().c_str());

		std::string extension { p.extension().generic_string() };
		if (std::experimental::filesystem::is_directory(p) || extension == ".scene" || extension != ".asset")
			return;

		const QFileSystemModel * test = dynamic_cast<const QFileSystemModel*>(kModel.model());
		std::string str{ test->filePath(kModel).toStdString() };

		clearToolBox();
		loadAsset(str);
	}

	void WInspector::updateScene(QTreeWidgetItem* current, QTreeWidgetItem* previous)
	{
		(void) previous;

		if (!current)
		{
			Service::get<gizmo::GizmoHandler>()->selectActor(nullptr);
			mCurrentActor = nullptr;
			return;
		}

		int32_t column { current->treeWidget()->currentColumn() };
		std::string name { current->data(column, Qt::DisplayRole).toString().toStdString() };

		scene::Scene* activeScene { Service::get<scene::SceneHandler>()->activeScene() };
		scene::Actor* actor { activeScene->actor(name) };
		if (!actor || mCurrentActor == actor)
			return;

		mCurrentActor = actor;
		Service::get<gizmo::GizmoHandler>()->selectActor(mCurrentActor);
		loadActor();
	}

	void WInspector::selectionChanged()
	{
		clearToolBox();
		Service::get<gizmo::GizmoHandler>()->selection().clear();
	}

	void WInspector::clearToolBox()
	{
		while (mTool->count() != 0)
			delete mTool->widget(0);
		mCurrentActor = nullptr;
	}

	bool WInspector::inspectActor() const
	{
		return mCurrentActor != nullptr;
	}

	void WInspector::loadActor()
	{
		if (!mCurrentActor)
			return;

		if (WInspectorComponent::registeredInspector()[mCurrentActor->hash()])
		{
			QWidget* val{ WInspectorComponent::registeredInspector()[mCurrentActor->hash()](mCurrentActor, this, nullptr) };
			mTool->addItem(val, val->objectName());
		}
		setWindowTitle(("Inspector " + mCurrentActor->name()).c_str());

		scene::CTransform* rootTransform{ mCurrentActor->rootTransform() };
		if (rootTransform)
		{
			QWidget* rootTr { WInspectorComponent::registeredInspector()[rootTransform->hash()](rootTransform, this, nullptr) };
			mTransfIndex = mTool->count();
			mTool->addItem(rootTr, rootTr->objectName());
		}

		uint32_t compCount { mCurrentActor->componentCount() };
		for (uint32_t nbComp { 0u } ; nbComp < compCount ; nbComp++)
		{
			scene::IComponent* component { mCurrentActor->components()[nbComp] };
			if (WInspectorComponent::registeredInspector()[component->hash()])
			{
				QWidget* componentWidget { WInspectorComponent::registeredInspector()[component->hash()](component, this, nullptr) };
				mTool->addItem(componentWidget, componentWidget->objectName());
			}
			else
				LOG(LogWarning, "Actor " << mCurrentActor->name() << " component " << nbComp << " doesn't have a registered inspector")
		}
	}

	void WInspector::loadAsset(const std::string& kName)
	{
		setWindowTitle(("Inspector " + kName.substr(kName.rfind('/') + 1u)).c_str());

		std::unique_ptr<JsonNode> root{ JsonNode::parse(kName.c_str()) };
		uint32_t type { (*root)["type"]->getUint32() };
		uint32_t guid { (*root)["mGuid"]->getUint32() };

		if (WInspectorAsset::registeredAssetInspector()[type])
		{
			WInspectorAsset* asset { WInspectorAsset::registeredAssetInspector()[type](guid, this, nullptr) };
			mTool->addItem(asset, asset->objectName());
			asset->setObjectName(kName.c_str());
			asset->setDisabled(mParent->isRunning());
		}
	}

	void WInspector::showContextMenu(const QPoint& kPos)
	{
		if (!mCurrentActor)
			return;

		QMenu contextMenu {};
		QMenu addActor { "Add Component" , &contextMenu };
		contextMenu.addMenu(&addActor);

		QAction* addCTransform { addActor.addAction("CTransform") };

		const std::unordered_map<uint32_t, WInspectorComponent::ComponentInfo>& kComponents { WInspectorComponent::registeredComponents() };
		for (const std::pair<const uint32_t, WInspectorComponent::ComponentInfo>& kPair : kComponents)
		{
			const std::string& kName { kPair.second.mName };

			QAction* addAction { addActor.addAction(kName.substr(kName.rfind(':') + 1u).c_str()) };
			addAction->connect(addAction, &QAction::triggered, this, [this, kPair] () -> void
							   {
								   if (!WInspectorComponent::registeredInspector()[kPair.first])
									   return;

								   QWidget* component { WInspectorComponent::registeredInspector()[kPair.first](kPair.second.mCreateComponent(mCurrentActor), mTool, nullptr) };
								   mTool->addItem(component, component->objectName());
							   });
		}

		QAction* remove { contextMenu.addAction("Remove Component") };

		addCTransform->connect(addCTransform, &QAction::triggered, this, &WInspector::addTransform);
		remove->connect(remove, &QAction::triggered, this, &WInspector::removeComponent);

		contextMenu.exec(mapToGlobal(kPos));
	}

	void WInspector::addTransform()
	{
		if (!mCurrentActor || mCurrentActor->rootTransform())
			return;

		mCurrentActor->rootTransform({});
		if (WInspectorComponent::registeredInspector()[meta::Class<scene::CTransform>::hash()])
		{
			QWidget* rootTr { WInspectorComponent::registeredInspector()[meta::Class<scene::CTransform>::hash()](mCurrentActor->rootTransform(), this, nullptr) };
			mTool->addItem(rootTr, rootTr->objectName());
			mTransfIndex = mTool->count() - 1;
		}
		else
			LOG(LogWarning, "CTransform doesn't have a registered inspector")
	}

	void WInspector::removeComponent()
	{
		if (!mCurrentActor)
			return;

		int32_t currentIndex { mTool->currentIndex() };
		if (currentIndex == 0 || currentIndex == mTransfIndex)
			return;
		else if (currentIndex > mTransfIndex)
			mCurrentActor->removeComponent(currentIndex - 2);
		else if (currentIndex < mTransfIndex)
		{ 
			mCurrentActor->removeComponent(currentIndex - 1);
			mTransfIndex--;
		}

		mTool->removeItem(currentIndex);
	}

	void WInspector::canRighClick()
	{
		if(!mParent->isRunning())
			mTool->connect(mTool, &QToolBox::customContextMenuRequested, this, &WInspector::showContextMenu);
		else
			mTool->disconnect(mTool, &QToolBox::customContextMenuRequested, this, &WInspector::showContextMenu);
	}
}