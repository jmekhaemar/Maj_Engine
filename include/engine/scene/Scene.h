#ifndef _SCENE_H_
#define _SCENE_H_

#include <NonCopyable.h>
#include <ISerializable.h>

#include "scene/Actor.h"
#include "scene/LightHandler.h"
#include "scene/CameraHandler.h"

#ifdef MAJ_EDITOR
#include "scene/AEditorCamera.h"
#endif

namespace maj::scene
{
	class Scene final : public NonCopyable, public ISerializable
	{
		ENABLE_META(Scene)

	public:
		LOAD_CONSTRUCTOR_CUSTOM(Scene)

		Scene();
		Scene(const std::string& kStr, const uint16_t order);
		~Scene();

		const std::string& name() const;
		void name(const char* kStr);

		uint16_t order() const;

		template <typename T, typename... Args>
		T* addActor(const std::string& kStr, Args&&... args);

		void removeActors();
		void removeActor(Actor* actor);

		const Actor* actor(const std::string& kStr) const;
		Actor* actor(const std::string& kStr);

		const Actor* actor(const Actor* kParent, const std::string&) const;
		Actor* actor(Actor* parent, const std::string&);

		template<typename T>
		const T* firstComponent(const Actor* node) const;
		template<typename T>
		const T* firstComponent() const;
		template<typename T>
		T* firstComponent(Actor* node);
		template<typename T>
		T* firstComponent();

		void fixedUpdate();
		void update(float deltaTime, bool isSimulation);

		const CameraHandler& cameras() const;
		CameraHandler& cameras();

		const LightHandler& lights() const;
		LightHandler& lights();

		Event<void, const rhi::IDescriptor*> mOnCameraChange;

	private:
		void fixedUpdateActor(Actor* actualActor);
		void updateActor(Actor* actualActor, float deltaTime, bool isSimulation);
		void lateUpdateActor(Actor* actualActor, float deltaTime, bool isSimulation);

		std::string mName;
		uint16_t mOrder;

		Actor* mRoot;

		CameraHandler mCameras;
		LightHandler mLights;

#ifdef MAJ_EDITOR
		AEditorCamera* mCamera;
#endif
	};

}

META_CLASS(maj::scene::Scene, 3u)
META_FIELD(maj::scene::Scene, std::string, mName, 0u)
META_FIELD(maj::scene::Scene, maj::scene::Actor*, mRoot, 1u)
META_FIELD(maj::scene::Scene, uint16_t, mOrder, 2u)

#include "scene/Scene.inl"

#endif