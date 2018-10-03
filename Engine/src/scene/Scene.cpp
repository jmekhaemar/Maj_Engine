#include "scene/Scene.h"

#include "scene/SceneHandler.h"
#include "scene/AGameCamera.h"
#include "scene/ASkybox.h"
#include "scene/CDirectionalLight.h"
#include "Engine.h"

namespace maj::scene
{
	DEFINE_HASH_OVERLOAD(Scene)

	Scene::Scene(JsonNode* root)
		: mName { "NoName" }, mOrder { 0u }, mRoot { nullptr }, mCameras {}, mLights {}
	{
#ifdef MAJ_EDITOR
		mCamera = nullptr;
#endif

		load(root);
		initialize();
	}

	Scene::Scene()
		: mName { "Default" }, mOrder { 0u }, mRoot { nullptr }, mCameras {}, mLights {}
	{
#ifdef MAJ_EDITOR
		mCamera = nullptr;
#endif
	}

	Scene::Scene(const std::string& kStr, uint16_t order)
		: mName { kStr }, mOrder { order }, mRoot { new Actor {} }, mCameras {}, mLights {}
	{
#ifdef MAJ_EDITOR
		mCamera = nullptr;
#endif
	}

	Scene::~Scene()
	{
#ifdef MAJ_EDITOR
		if (mCamera)
			delete mCamera;
#endif

		if (mRoot)
			delete mRoot;
	}

	void Scene::initialize()
	{
		mRoot->awake();

		// Load a default scene
		if (mRoot->childCount() == 0)
		{
			addActor<AGameCamera>("Camera");
			addActor<ASkybox>("Skybox", Asset<rhi::Material>::get("asset/default/skyboxMaterial"));
			Actor* light { addActor<Actor>("Light") };
			light->addComponent<CDirectionalLight>();
			light->rootTransform({ { 5.f, 3.f, 0.f }, math::Quatf::fromEuler(22.5f, -90.0f, 0.f),{ 0.1f, 0.1f, 0.1f } });
		}

#ifdef MAJ_EDITOR
		if (!mCamera)
			mCamera = new AEditorCamera { "EditorCamera" };

		mCamera->awake();
#endif

#ifdef MAJ_EDITOR
		if (Service::get<Engine>()->isSimulating())
			mCameras.main(firstComponent<CCamera>());
#endif
	}

	const std::string& Scene::name() const
	{
		return mName;
	}

	void Scene::name(const char* kStr)
	{
		if (mName == kStr)
			return;

		const std::string kName { std::move(mName) };
		mName = kStr;
		Service::get<SceneHandler>()->rename(kName.c_str(), kStr);
	}

	uint16_t Scene::order() const
	{
		return mOrder;
	}

	void Scene::removeActors()
	{
#ifdef MAJ_EDITOR
		if (mCamera)
		{
			delete mCamera;
			mCamera = nullptr;
		}
#endif

		delete mRoot;
		mRoot = nullptr;
	}

	void Scene::removeActor(Actor* actor)
	{
		if (!actor)
			return;

		if (actor == mRoot)
		{
			LOG(LogWarning, "failed to remove actor: can't remove the root actor in scene \'" << mName << "\'")
			return;
		}
		
		actor->parent()->removeChild(actor);
	}
	
	const Actor* Scene::actor(const std::string& kStr) const
	{
		if (kStr == "root")
		{ 
			LOG(LogWarning, "failed to get actor: can't get the root actor ")
			return nullptr;
		}
		const Actor* temp { actor(mRoot, kStr) };
		if (!temp)
			LOG(LogWarning, "failed to get actor: actor \'" << kStr << "\' in scene \'" << mName << "\' not found")

		return temp;
	}

	Actor* Scene::actor(const std::string& kStr)
	{
		if (kStr == "root")
		{
			LOG(LogWarning, "failed to get actor: can't get the root actor ")
			return nullptr;
		}
		Actor* temp { actor(mRoot, kStr) };
		if (!temp)
			LOG(LogWarning, "failed to get actor: actor \'" << kStr << "\' in scene \'" << mName << "\' not found")

		return temp;
	}

	const Actor* Scene::actor(const Actor* kParent, const std::string& kStr) const
	{
		if (!kParent)
			return nullptr;

		if (kParent->name() == kStr)
			return kParent;

		uint8_t childCount { kParent->childCount() };
		for (uint8_t index { 0u }; index < childCount; ++index)
		{
			const Actor* temp { actor(kParent->child(index), kStr) };
			if (temp)
				return temp;
		}

		return nullptr;
	}

	Actor* Scene::actor(Actor* parent, const std::string& kStr)
	{
		if (!parent)
			return nullptr;

		if (parent->name() == kStr)
			return parent;

		uint8_t childCount { parent->childCount() };
		for (uint8_t index{ 0u }; index < childCount; ++index)
		{
			Actor* temp { actor(parent->child(index), kStr) };
			if (temp)
				return temp;
		}

		return nullptr;
	}

	void Scene::updateActor(Actor* actualActor, float deltaTime, bool isSimulation)
	{
		if (!actualActor)
			return;

		uint8_t childrenSize{ actualActor->childCount() };
		for (uint8_t index { 0u } ; index < childrenSize ; ++index)
		{
			if (isSimulation)
				actualActor->child(index)->mOnUpdate(static_cast<float&&>(deltaTime));

			updateActor(actualActor->child(index), deltaTime, isSimulation);
		}
	}

	void Scene::lateUpdateActor(Actor* actualActor, float deltaTime, bool isSimulation)
	{
		if (!actualActor)
			return;

		uint8_t childrenSize{ actualActor->childCount() };
		for (uint8_t index{ 0u }; index < childrenSize; ++index)
		{
			if (isSimulation)
				actualActor->child(index)->mOnLateUpdate(static_cast<float&&>(deltaTime));

			actualActor->child(index)->mOnDraw();

			lateUpdateActor(actualActor->child(index), deltaTime, isSimulation);
		}
	}

	void Scene::fixedUpdateActor(Actor* actualActor)
	{
		if (!actualActor)
			return;

		uint8_t childrenSize{ actualActor->childCount() };
		for (uint8_t index{ 0u }; index < childrenSize; ++index)
		{
			actualActor->child(index)->mOnFixedUpdate();
			fixedUpdateActor(actualActor->child(index));
		}
	}

	void Scene::fixedUpdate()
	{
		fixedUpdateActor(mRoot);

#ifdef MAJ_EDITOR
		Service::get<gizmo::GizmoHandler>()->fixedUpdate();
#endif
	}

	void Scene::update(float deltaTime, bool isSimulation)
	{
		updateActor(mRoot, deltaTime, isSimulation);
#ifdef MAJ_EDITOR
		gizmo::GizmoHandler* gizmos { Service::get<gizmo::GizmoHandler>() };
		gizmos->update(deltaTime);
#endif

		lateUpdateActor(mRoot, deltaTime, isSimulation);
#ifdef MAJ_EDITOR
		gizmos->lateUpdate(deltaTime);
		gizmos->draw();

		if (!isSimulation && mCamera)
		{
			mCamera->mOnUpdate(static_cast<float&&>(deltaTime));
			mCamera->mOnLateUpdate(static_cast<float&&>(deltaTime));
			mCamera->mOnDraw();
		}
#endif

		mLights.draw();
	}

	const CameraHandler& Scene::cameras() const
	{
		return mCameras;
	}

	CameraHandler& Scene::cameras()
	{
		return mCameras;
	}

	const LightHandler& Scene::lights() const
	{
		return mLights;
	}

	LightHandler& Scene::lights()
	{
		return mLights;
	}
}