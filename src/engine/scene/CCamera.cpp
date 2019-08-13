#include "scene/CCamera.h"

#include <Helper.h>
#include <Logging.h>
#include <Window.h>

#include "scene/CTransform.h"
#include "scene/SceneHandler.h"

#ifdef MAJ_EDITOR
#include "gizmo/Sprite.h"
#endif

namespace maj::scene
{
	DEFINE_HASH_OVERLOAD(CCamera)

	CCamera::CCamera(Actor* actor, float fovh, float pnear, float pfar)
		: IComponent{ actor }, mFovh{ fovh }, mNear{ pnear }, mFar{ pfar }, mRatio{ 0.0f }, mBuffer{ nullptr }, mDrawEventIndex{ 0u }
	{
		
		ww::Viewport& viewport{ Service::get<ww::Window>()->viewport(0u) };
		mRatio = static_cast<float>(viewport.extent().mX) / static_cast<float>(viewport.extent().mY);

		initialize();
		onAttach(nullptr);
	}

	CCamera::CCamera(Actor* actor, float fovh, float pnear, float pfar, float ratio)
		: IComponent { actor }, mFovh { fovh }, mNear { pnear }, mFar { pfar }, mRatio { ratio }, mBuffer { nullptr }, mDrawEventIndex { 0u }
	{
		initialize();
		onAttach(nullptr);
	}

	CCamera::~CCamera()
	{
		rhi::Buffer::release(mBuffer);
	}

	void CCamera::initialize()
	{
		// Enable/Disable
		mOnEnabled += [this] () -> void
		{
			mDrawEventIndex = mActor->mOnDraw + std::bind(&CCamera::draw, this);

			Scene* scene { Service::get<SceneHandler>()->activeScene() };
			if (scene)
				scene->cameras().add(this);

#ifdef MAJ_EDITOR
			Service::get<gizmo::GizmoHandler>()->enable(this, true);
#endif
		};
		
		mOnDisabled += [this] (bool destroyed) -> void
		{
			(void) destroyed;

			if (mActor)
				mActor->mOnDraw -= mDrawEventIndex;

			Scene* scene { Service::get<SceneHandler>()->activeScene() };
			if (scene)
				scene->cameras().remove(this);

#ifdef MAJ_EDITOR
			if (destroyed)
				Service::get<gizmo::GizmoHandler>()->remove(this);
			else
				Service::get<gizmo::GizmoHandler>()->enable(this, false);
#endif
		};

		// Projection
		mBuffer = rhi::Buffer::request(rhi::Buffer::EType::UNIFORM, sizeof(math::Mat4f) * 2u);

		projection(mRatio);

		// Main camera
		Scene* scene { Service::get<SceneHandler>()->activeScene() };
		if (mIsEnabled && scene)
			scene->cameras().add(this);
	}

	void CCamera::draw() const
	{
		const CTransform* kActorTr { mActor->rootTransform() };
		if (!kActorTr)
		{
			LOG(LogWarning, "failed to draw camera: actor \'" << mActor->name() << "\' has no root transform")
			return;
		}

		rhi::Buffer::MemoryMap memMap { *mBuffer };

		math::Transf invTr { kActorTr->global().inversed() };

		std::memcpy(memMap.mData, mProjection.elem(), sizeof(math::Mat4f));
		std::memcpy(memMap.mData + sizeof(math::Mat4f), invTr.matrix().elem(), sizeof(math::Mat4f));
	}
	
	void CCamera::lookAt(const math::Vec3f& kEye, const math::Vec3f& kLookAt, const math::Vec3f& kUp)
	{
		CTransform* actorTr { mActor->rootTransform() };
		if (!actorTr)
		{
			LOG(LogWarning, "failed to set camera look at: actor \'" << mActor->name() << "\' has no root transform")
			return;
		}

		math::Transf transform { actorTr->transform() };
		transform.position(kEye);
		transform.lookAt(kLookAt, kUp);

		actorTr->transform(transform);
	}

	void CCamera::projection(float ratio)
	{
		float t { tanf(mFovh * DEG2RAD * 0.5f) };

		mProjection.at(0, 0) = 1.f / (t * ratio);
		mProjection.at(1, 1) = -(1.f / t);
		mProjection.at(2, 2) = mFar / (mFar - mNear);
		mProjection.at(2, 3) = 1.f;
		mProjection.at(3, 2) = -(mNear * mFar) / (mFar - mNear);
	}

	void CCamera::projection(float fovh, float pNear, float pFar, float ratio)
	{
		mFovh = fovh;
		mNear = pNear;
		mFar = pFar;

		projection(ratio);
	}

	float CCamera::fovh() const
	{
		return mFovh;
	}

	float CCamera::nearPlane() const
	{
		return mNear;
	}

	float CCamera::farPlane() const
	{
		return mFar;
	}

	const rhi::Buffer::SubBuffer* CCamera::buffer() const
	{
		return mBuffer;
	}

	void CCamera::awake()
	{
		IComponent::awake();

#ifdef MAJ_EDITOR
		if (!Asset<rhi::Image>::exists("asset/default/imgCamera"))
			Asset<rhi::Image>::add("asset/default/imgCamera", "data/textures/Camera.png",
								   maj::rhi::Format { maj::rhi::Format::EComponent::R8G8B8A8, maj::rhi::Format::EType::UNORM });

		if (!Asset<rhi::Texture>::exists("asset/default/camera"))
			maj::Asset<maj::rhi::Texture>::add("asset/default/camera", maj::Asset<maj::rhi::Image>::get("asset/default/imgCamera"));

		gizmo::GizmoHandler* gizmoHandler { Service::get<gizmo::GizmoHandler>() };
		gizmoHandler->add<gizmo::Sprite>(this, Asset<rhi::Texture>::get("asset/default/camera"));
#endif
	}

	void CCamera::onAttach(Actor* previousActor)
	{
		if (previousActor)
			previousActor->mOnDraw -= mDrawEventIndex;

		if (mIsEnabled)
			mDrawEventIndex = mActor->mOnDraw + std::bind(&CCamera::draw, this);
	}
}
