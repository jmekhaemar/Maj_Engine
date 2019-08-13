#include "scene/AEditorCamera.h"

#include <Window.h>

#ifdef MAJ_EDITOR
#include "Engine.h"
#endif

namespace maj::scene
{
	DEFINE_HASH_OVERLOAD(AEditorCamera)

	AEditorCamera::AEditorCamera(const std::string& kName, uint8_t viewportIndex)
		: ACamera { kName, viewportIndex }, mSpeed { 5.f }, mMouseSpeed { 0.05f }, mZoomSpeed { 200.f }, mEuler {}, mStartSimIndex { 0u }, mStopSimIndex { 0u }
	{
		initialize();
	}

	AEditorCamera::~AEditorCamera()
	{
#ifdef MAJ_EDITOR
		Engine* engine { Service::get<Engine>() };
		engine->mOnStartSimulation -= mStartSimIndex;
		engine->mOnStopSimulation -= mStopSimIndex;
#endif
	}

	void AEditorCamera::initialize()
	{
		mOnUpdate += std::bind(&AEditorCamera::update, this, std::placeholders::_1);

#ifdef MAJ_EDITOR
		Engine* engine { Service::get<Engine>() };
		mStartSimIndex = engine->mOnStartSimulation + [this] () -> void
		{
			mCamera->enable(false);
		};
		mStopSimIndex = engine->mOnStopSimulation + [this] () -> void
		{
			mCamera->enable(true);

			Scene* activeScene { Service::get<SceneHandler>()->activeScene() };
			activeScene->cameras().main(mCamera);
		};
#endif
	}

	float AEditorCamera::speed() const
	{
		return mSpeed;
	}

	void AEditorCamera::speed(float speed)
	{
		mSpeed = speed;
	}

	float AEditorCamera::mouseSpeed() const
	{
		return mMouseSpeed;
	}

	void AEditorCamera::mouseSpeed(float speed)
	{
		mMouseSpeed = speed;
	}

	float AEditorCamera::zoomSpeed() const
	{
		return mZoomSpeed;
	}

	void AEditorCamera::zoomSpeed(float speed)
	{
		mZoomSpeed = speed;
	}

	void AEditorCamera::lookAt(const math::Vec3f& kPos, const math::Vec3f& kLookAt, const math::Vec3f& kUp)
	{
		ACamera::lookAt(kPos, kLookAt, kUp);
		mEuler = mRootTransform->rotation().toEuler();
	}

	void AEditorCamera::awake()
	{
		ACamera::awake();

		if (!mRootTransform)
			mRootTransform = new CTransform { this };

		mCamera = component<CCamera>();
		if (!mCamera)
		{
			ww::Viewport& viewport { Service::get<ww::Window>()->viewport(mViewportIndex) };
			const math::Vec2ui& kWindowExtent { viewport.extent() };
			mCamera = addComponent<scene::CCamera>(45.f, 0.1f, 100.f, static_cast<float>(kWindowExtent.mX) / static_cast<float>(kWindowExtent.mY));
		}

		mEuler = mRootTransform->rotation().toEuler();

#ifdef MAJ_EDITOR
		Scene* activeScene { Service::get<SceneHandler>()->activeScene() };
		if (activeScene)
			activeScene->cameras().main(mCamera);
#endif
	}

	void AEditorCamera::update(float deltaTime)
	{
		const ww::Input* kInput { Service::get<ww::Input>() };

		math::Transf camTr { mRootTransform->transform() };
		math::Vec3f newPos { camTr.position() };
		const math::Vec3f kForward { camTr.forward() }, kRight { camTr.right() }, kUp { camTr.up() };

		// Position
		float moveSpeed { mSpeed * deltaTime };
		if (kInput->key(ww::EKey::W).isDown())
			newPos += kForward * moveSpeed;
		if (kInput->key(ww::EKey::S).isDown())
			newPos -= kForward * moveSpeed;
		if (kInput->key(ww::EKey::A).isDown())
			newPos -= kRight * moveSpeed;
		if (kInput->key(ww::EKey::D).isDown())
			newPos += kRight * moveSpeed;
		if (kInput->key(ww::EKey::Q).isDown())
			newPos -= kUp * moveSpeed;
		if (kInput->key(ww::EKey::E).isDown())
			newPos += kUp * moveSpeed;

		float scroll { kInput->mouse().scroll() };
		if (!math::Helper<float>::areSame(scroll, 0.f))
			newPos += kForward * scroll * mZoomSpeed * deltaTime;

		camTr.position(newPos);

		// Rotation
		if (kInput->mouse().key(ww::EMouseKey::RIGHT).isDown())
		{
			const math::Vec2f& deltaAngle { kInput->mouse().relativePosition() * mMouseSpeed };
			mEuler.mX -= deltaAngle.mY;
			mEuler.mY -= deltaAngle.mX;

			camTr.rotation(math::Quatf::fromEuler(mEuler));
		}

		mRootTransform->transform(camTr);
	}
}