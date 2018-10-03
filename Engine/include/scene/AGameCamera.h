#ifndef _AGAME_CAMERA_H_
#define _AGAME_CAMERA_H_

#include "scene/ACamera.h"

namespace maj::scene
{
	class AGameCamera final : public ACamera
	{
		ENABLE_META_PARENT(AGameCamera, Actor)

	public:
		LOAD_CONSTRUCTOR(AGameCamera)

		AGameCamera(const std::string& kName, uint8_t viewportIndex = 0u);
		virtual ~AGameCamera() = default;

		float speed() const;
		void speed(float speed);

		float mouseSpeed() const;
		void mouseSpeed(float speed);

		virtual void lookAt(const math::Vec3f& kPos, const math::Vec3f& kLookAt, const math::Vec3f& kUp = math::Vec3f::skUp) override;

		virtual void awake() override;

	private:
		void update(float deltaTime);

		float mSpeed;
		float mMouseSpeed;
		math::Vec3f mEuler;
	};
}

META_CLASS_PARENT(maj::scene::AGameCamera, maj::scene::ACamera, 2u)
META_FIELD(maj::scene::AGameCamera, float, mSpeed, 0u)
META_FIELD(maj::scene::AGameCamera, float, mMouseSpeed, 1u)

#endif