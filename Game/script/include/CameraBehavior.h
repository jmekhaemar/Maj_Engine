#ifndef _CAMERA_BEHAVIOR_H_
#define _CAMERA_BEHAVIOR_H_

#include <scene/CScript.h>
#include <physic/ICCollider.h>

#include "Project.h"

namespace maj::script
{ 
	class CameraBehavior final : public scene::CScript
	{
		ENABLE_META_PARENT(CameraBehavior, scene::CScript)

	public:
		LOAD_CONSTRUCTOR(CameraBehavior)

		CameraBehavior(scene::Actor* actor);
		virtual ~CameraBehavior() = default;

		virtual void update(float deltaTime) override;
		virtual void fixedUpdate() override;

	private:
		physic::ICCollider* mHoldObject;

		float mSpeed;
		float mMaxAngleX;

		math::Vec3f mEuler;
	};

	REGISTER_COMPONENT_INSPECTOR(CameraBehavior)
}

META_CLASS_PARENT(maj::script::CameraBehavior, maj::scene::CScript, 2u)
META_FIELD(maj::script::CameraBehavior, float, mSpeed, 0u)
META_FIELD(maj::script::CameraBehavior, float, mMaxAngleX, 1u)

#endif