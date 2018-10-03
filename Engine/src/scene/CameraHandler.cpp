#include "scene/CameraHandler.h"

#include "scene/SceneHandler.h"

namespace maj::scene
{
	uint8_t CameraHandler::skMaxCameraCount { 255u };

	CameraHandler::CameraHandler()
		: mCameras {}, mMainIndex { skMaxCameraCount }
	{}

	void CameraHandler::add(CCamera* camera)
	{
		if (!camera)
			return;

		// Check if already contained
		for (const CCamera* kCamera : mCameras)
		{
			if (kCamera == camera)
				return;
		}

		uint8_t cameraCount { static_cast<uint8_t>(mCameras.size()) };
		if (cameraCount == skMaxCameraCount)
		{
			LOG(LogWarning, "failed to add camera: max camera count exceeded")
			return;
		}

		mCameras.emplace_back(camera);
		if (cameraCount == 0u)
		{
			mMainIndex = 0u;
			Scene* scene { Service::get<SceneHandler>()->activeScene() };
			scene->mOnCameraChange(mCameras[mMainIndex]->buffer());
		}
	}

	void CameraHandler::remove(const CCamera* kCamera)
	{
		// Find if contained in the handler
		uint8_t cameraCount { static_cast<uint8_t>(mCameras.size()) };
		uint8_t index { skMaxCameraCount };
		for (uint8_t i { 0u } ; i < cameraCount && index == skMaxCameraCount ; ++i)
		{
			if (mCameras[i] != kCamera)
				continue;

			index = i;
		}

		if (index == skMaxCameraCount)
			return;

		if (mMainIndex == index)
		{
			if (cameraCount < 2u)
			{
				LOG(LogWarning, "deleting the last camera")
				mMainIndex = skMaxCameraCount;
			}
			else
			{
				mMainIndex = 0u;
				Scene* scene { Service::get<SceneHandler>()->activeScene() };
				scene->mOnCameraChange(mCameras[mMainIndex]->buffer());
			}
		}

		mCameras.erase(mCameras.begin() + index);
	}

	uint8_t CameraHandler::count() const
	{
		return static_cast<uint8_t>(mCameras.size());
	}

	const CCamera* CameraHandler::main() const
	{
		if (mMainIndex == skMaxCameraCount)
			return nullptr;

		return mCameras[mMainIndex];
	}

	CCamera* CameraHandler::main()
	{
		if (mMainIndex == skMaxCameraCount)
			return nullptr;

		return mCameras[mMainIndex];
	}

	void CameraHandler::main(const CCamera* kCamera)
	{
		// Find if contained in the handler
		uint8_t cameraCount { static_cast<uint8_t>(mCameras.size()) };
		uint8_t index { skMaxCameraCount };
		for (uint8_t i { 0u } ; i < cameraCount && index == skMaxCameraCount ; ++i)
		{
			if (mCameras[i] != kCamera)
				continue;

			index = i;
		}

		if (index == skMaxCameraCount)
			return;

		if (mMainIndex == index)
			return;

		mMainIndex = index;
		Scene* scene { Service::get<SceneHandler>()->activeScene() };
		scene->mOnCameraChange(mCameras[mMainIndex]->buffer());
	}

	void CameraHandler::main(std::nullptr_t null)
	{
		(void) null;

		uint8_t cameraCount { static_cast<uint8_t>(mCameras.size()) };
		mMainIndex = cameraCount == 0u ? skMaxCameraCount : 0u;
	}
}