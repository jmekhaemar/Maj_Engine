#ifndef _CCAMERA_H_
#define _CCAMERA_H_

#include <Buffer.h>
#include <Mat4.h>

#include "scene/IComponent.h"

namespace maj::scene
{
	class CCamera final : public IComponent
	{
		ENABLE_META_PARENT(CCamera, IComponent)

	public:
		LOAD_CONSTRUCTOR(CCamera)

		CCamera(Actor* actor, float fovh = 45.0f, float pnear = 0.1f, float pfar = 100.0f);
		CCamera(Actor* actor, float fovh, float pnear, float pfar, float ratio);
		~CCamera();


		void draw() const;

		void lookAt(const math::Vec3f& kEye, const math::Vec3f& kLookAt, const math::Vec3f& kUp = math::Vec3f::skUp);
		void projection(float ratio);
		void projection(float fovh, float pnear, float pfar, float ratio);

		float fovh() const;
		float nearPlane() const;
		float farPlane() const;

		const rhi::Buffer::SubBuffer* buffer() const;

		virtual void awake() override;

	private:
		virtual void onAttach(Actor* previousActor) override;

		math::Mat4f mProjection;
		
		float mFovh;
		float mNear;
		float mFar;
		float mRatio;

		rhi::Buffer::SubBuffer* mBuffer;
		uint32_t mDrawEventIndex;
	};
}

META_CLASS_PARENT(maj::scene::CCamera, maj::scene::IComponent, 4u)
META_FIELD(maj::scene::CCamera, float, mFovh, 0u)
META_FIELD(maj::scene::CCamera, float, mNear, 1u)
META_FIELD(maj::scene::CCamera, float, mFar, 2u)
META_FIELD(maj::scene::CCamera, float, mRatio, 3u)

#endif