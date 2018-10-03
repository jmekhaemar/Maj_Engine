#ifndef _ACAMERA_H_
#define _ACAMERA_H_

#include <Vec2.h>

#include "scene/Actor.h"
#include "scene/CCamera.h"

namespace maj::scene
{
	class ACamera : public Actor
	{
		ENABLE_META_PARENT(ACamera, Actor)

	public:
		ACamera(const std::string& kName, uint8_t viewportIndex = 0u);
		virtual ~ACamera();

		INITIALIZE_SIGNATURE();

		const CCamera* camera() const;
		CCamera* camera();
		
		virtual void lookAt(const math::Vec3f& kPos, const math::Vec3f& kLookAt, const math::Vec3f& kUp = math::Vec3f::skUp);

	protected:
		ACamera();

		void resize(const math::Vec2ui& kExtent);

		CCamera* mCamera;
		uint8_t mViewportIndex;

	private:
		uint32_t mResizeEventIndex;
	};
}

META_CLASS_PARENT(maj::scene::ACamera, maj::scene::Actor, 1u)
META_FIELD(maj::scene::ACamera, uint8_t, mViewportIndex, 0u)

#endif