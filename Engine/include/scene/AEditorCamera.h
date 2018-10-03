#ifndef _AEDITOR_CAMERA_H_
#define _AEDITOR_CAMERA_H_

#include "scene/ACamera.h"

namespace maj::scene
{
	class AEditorCamera final : public ACamera
	{
		ENABLE_META_PARENT(AEditorCamera, Actor)

	public:
		LOAD_CONSTRUCTOR(AEditorCamera)
			
		AEditorCamera(const std::string& kName, uint8_t viewportIndex = 0u);
		virtual ~AEditorCamera();

		float speed() const;
		void speed(float speed);

		float mouseSpeed() const;
		void mouseSpeed(float speed);

		float zoomSpeed() const;
		void zoomSpeed(float speed);

		virtual void lookAt(const math::Vec3f& kPos, const math::Vec3f& kLookAt, const math::Vec3f& kUp = math::Vec3f::skUp) override;

		virtual void awake() override;

	private:
		void update(float deltaTime);

		float mSpeed;
		float mMouseSpeed;
		float mZoomSpeed;
		math::Vec3f mEuler;

		uint32_t mStartSimIndex;
		uint32_t mStopSimIndex;
	};
}

META_CLASS_PARENT(maj::scene::AEditorCamera, maj::scene::ACamera, 2u)
META_FIELD(maj::scene::AEditorCamera, float, mSpeed, 0u)
META_FIELD(maj::scene::AEditorCamera, float, mMouseSpeed, 1u)
META_FIELD(maj::scene::AEditorCamera, float, mZoomSpeed, 2u)

#endif