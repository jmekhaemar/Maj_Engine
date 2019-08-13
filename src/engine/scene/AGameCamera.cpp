#include "scene/AGameCamera.h"

#include <Window.h>

namespace maj::scene
{
	DEFINE_HASH_OVERLOAD(AGameCamera)

	AGameCamera::AGameCamera(const std::string& kName, uint8_t viewportIndex)
		: ACamera { kName, viewportIndex }, mSpeed { 5.f }, mMouseSpeed { 0.1f }, mEuler {}
	{
		initialize();
	}

	void AGameCamera::initialize()
	{
		mOnUpdate += std::bind(&AGameCamera::update, this, std::placeholders::_1);

		mCamera = component<CCamera>();
		if (!mCamera)
		{
			ww::Viewport& viewport{ Service::get<ww::Window>()->viewport(mViewportIndex) };
			const math::Vec2ui& kWindowExtent{ viewport.extent() };
			mCamera = addComponent<scene::CCamera>(45.f, 0.1f, 100.f, static_cast<float>(kWindowExtent.mX) / static_cast<float>(kWindowExtent.mY));
		}

		if (!mRootTransform)
			rootTransform({});
	}

	float AGameCamera::speed() const
	{
		return mSpeed;
	}

	void AGameCamera::speed(float speed)
	{
		mSpeed = speed;
	}

	float AGameCamera::mouseSpeed() const
	{
		return mMouseSpeed;
	}

	void AGameCamera::mouseSpeed(float speed)
	{
		mMouseSpeed = speed;
	}

	void AGameCamera::lookAt(const math::Vec3f& kPos, const math::Vec3f& kLookAt, const math::Vec3f& kUp)
	{
		ACamera::lookAt(kPos, kLookAt, kUp);
		mEuler = mRootTransform->global().rotation().toEuler();
	}

	void AGameCamera::awake()
	{
		ACamera::awake();

		mEuler = mRootTransform->global().rotation().toEuler();

#ifndef MAJ_EDITOR
		ww::Viewport& viewport { Service::get<ww::Window>()->viewport(mViewportIndex) };
		viewport.cursorMode(ww::Cursor::EMode::FORCE);
#endif
	}

	void AGameCamera::update(float deltaTime)
	{
		const ww::Input* kInput { Service::get<ww::Input>() };

		math::Transf camTr { mRootTransform->transform() };
		math::Vec3f newPos { camTr.position() };
		const math::Vec3f kForward { camTr.forward() }, kRight { camTr.right() };

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

		camTr.position(newPos);

		// Rotation
		const math::Vec2f& deltaAngle { kInput->mouse().relativePosition() * mMouseSpeed };
		mEuler.mX -= deltaAngle.mY;
		mEuler.mY -= deltaAngle.mX;

		camTr.rotation(math::Quatf::fromEuler(mEuler));

		mRootTransform->transform(camTr);
	}
}