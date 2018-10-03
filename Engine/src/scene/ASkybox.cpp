#include "scene/ASkybox.h"

#include <Renderer.h>

#include "scene/SceneHandler.h"
#include "gizmo/Grid.h"

namespace maj::scene
{
	DEFINE_HASH_OVERLOAD(ASkybox)

	ASkybox::ASkybox(JsonNode* root)
		: Actor { "ASkybox" }, mMaterialInstance { nullptr }, mCameraEventIndex { 0u }
	{
		load(root);
		initialize();
	}

	ASkybox::ASkybox(const std::string& kName, const rhi::Material* kMaterial)
		: Actor { kName }, mMaterialInstance { new rhi::MaterialInstance { kMaterial } }, mCameraEventIndex { 0u }
	{
		initialize();
	}

	ASkybox::~ASkybox()
	{
		Scene* activeScene { Service::get<SceneHandler>()->activeScene() };
		if (activeScene)
			activeScene->mOnCameraChange -= mCameraEventIndex;

		if(mMaterialInstance)
			delete mMaterialInstance;
	}

	void ASkybox::initialize()
	{
		Scene* activeScene { Service::get<SceneHandler>()->activeScene() };
		mCameraEventIndex = activeScene->mOnCameraChange + [this] (const rhi::IDescriptor* buffer) -> void
		{
			mMaterialInstance->descriptor(0u, 0u, buffer);
			mMaterialInstance->update(0u, 0u);
		};

		Service::get<rhi::Renderer>()->frameGraph().add(mMaterialInstance, std::vector<rhi::DrawCall::VertexInput>{ { 1u, 36u }});
	}

	const rhi::MaterialInstance* ASkybox::material()const
	{
		return mMaterialInstance;
	}

	rhi::MaterialInstance* ASkybox::material()
	{
		return mMaterialInstance;
	}

	void ASkybox::material(const rhi::Material* kMaterial)
	{
		if (!kMaterial)
		{
			LOG(LogWarning, "failed to set skybox material: no material")
			return;
		}

		if (mMaterialInstance)
			delete mMaterialInstance;

		mMaterialInstance = new rhi::MaterialInstance { kMaterial };

		Scene* activeScene { Service::get<SceneHandler>()->activeScene() };
		mMaterialInstance->descriptor(0u, 0u, activeScene->cameras().main()->buffer());
		mMaterialInstance->update();
	}

	void ASkybox::awake()
	{
		Actor::awake();

		Scene* activeScene { Service::get<SceneHandler>()->activeScene() };
		mMaterialInstance->descriptor(0u, 0u, activeScene->cameras().main()->buffer());
		mMaterialInstance->update();

#ifdef MAJ_EDITOR
		if (!mRootTransform)
			mRootTransform = new CTransform { this };

		Service::get<gizmo::GizmoHandler>()->add<gizmo::Grid>(mRootTransform);
#endif
	}
}