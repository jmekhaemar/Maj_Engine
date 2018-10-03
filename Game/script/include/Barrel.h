#ifndef _BARREL_H_
#define _BARREL_H_

#include <scene/CScript.h>
#include <physic/CSphereCollider.h>

#include "Project.h"

namespace maj::script
{
	class Barrel final : public scene::CScript
	{
		ENABLE_META_PARENT(Barrel, scene::CScript)

	public:
		LOAD_CONSTRUCTOR_CUSTOM(Barrel)

		Barrel(scene::Actor* actor);
		virtual ~Barrel();

		virtual void awake() override;

		virtual void update(float deltaTime) override;

	private:
		void getCollider();

		physic::CSphereCollider* mCollider;
		uint32_t mColliderDisabled;
	};

	REGISTER_COMPONENT_INSPECTOR(Barrel)
}

META_CLASS_PARENT(maj::script::Barrel, maj::scene::CScript, 0u)

#endif