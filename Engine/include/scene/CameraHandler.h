#ifndef _CAMERA_HANDLER_H_
#define _CAMERA_HANDLER_H_

#include "scene/CCamera.h"

namespace maj::scene
{
	class CameraHandler final : public NonCopyable
	{
	private:
		static uint8_t skMaxCameraCount;

	public:
		CameraHandler();
		~CameraHandler() = default;

		void add(CCamera* camera);
		void remove(const CCamera* kCamera);

		uint8_t count() const;

		const CCamera* main() const;
		CCamera* main();
		void main(const CCamera* kCamera);
		void main(std::nullptr_t null);

	private:
		std::vector<CCamera*> mCameras;
		uint8_t mMainIndex;
	};
}

#endif