#include "scene/ACamera.h"

#include <Window.h>

namespace maj::scene
{
	DEFINE_HASH_OVERLOAD(ACamera)

	ACamera::ACamera()
		: Actor { "Camera" }, mCamera { nullptr }, mViewportIndex { 0u }, mResizeEventIndex { 0u }
	{}

	ACamera::ACamera(const std::string& kName, uint8_t viewportIndex)
		: Actor { kName }, mCamera { nullptr }, mViewportIndex { viewportIndex }, mResizeEventIndex { 0u }
	{
		initialize();
	}

	ACamera::~ACamera()
	{
		Service::get<ww::Window>()->viewport(mViewportIndex).mOnResized -= mResizeEventIndex;
	}
	
	void ACamera::initialize() 
	{
		ww::Viewport& viewport{ Service::get<ww::Window>()->viewport(mViewportIndex) };
		mResizeEventIndex = viewport.mOnResized + [this](uint32_t id, const math::Vec2ui& kExtent) -> void
		{
			(void) id;
			resize(kExtent);
		};
	}

	const CCamera* ACamera::camera() const
	{
		return mCamera;
	}
	
	CCamera* ACamera::camera()
	{
		return mCamera;
	}

	void ACamera::lookAt(const math::Vec3f& kPos, const math::Vec3f& kLookAt, const math::Vec3f& kUp)
	{
		if (!mCamera)
			return;

		mCamera->lookAt(kPos, kLookAt, kUp);
	}

	void ACamera::resize(const math::Vec2ui& kExtent)
	{
		if (!mCamera)
			return;

		mCamera->projection(static_cast<float>(kExtent.mX) / static_cast<float>(kExtent.mY));
	}
}