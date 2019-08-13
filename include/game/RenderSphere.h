#ifndef _RENDER_SPHERE_H_
#define _RENDER_SPHERE_H_

#include <scene/CScript.h>
#include <Buffer.h>

namespace maj::script
{
	class RenderSphere : public scene::CScript
	{
		ENABLE_META_PARENT(RenderSphere, scene::CScript)

		public:
		LOAD_CONSTRUCTOR(RenderSphere)

		RenderSphere(scene::Actor* actor);
		~RenderSphere();

		virtual void awake() override;

		private:
		rhi::Buffer::SubBuffer* mBuffer;

		math::Vec3f mColor;
		float mMetallic;
		float mRoughness;
	};
}

META_CLASS_PARENT(maj::script::RenderSphere, maj::scene::CScript, 3u)
META_FIELD(maj::script::RenderSphere, maj::math::Vec3f, mColor, 0u)
META_FIELD(maj::script::RenderSphere, float, mMetallic, 1u)
META_FIELD(maj::script::RenderSphere, float, mRoughness, 2u)

#endif
