#ifndef _DEATH_ZONE_H_
#define _DEATH_ZONE_H_

#include <scene/CScript.h>
#include <physic/CSphereCollider.h>

#include "Project.h"

namespace maj::script
{
	class DeathZone final : public scene::CScript
	{
		ENABLE_META_PARENT(DeathZone, scene::CScript)

	public:
		LOAD_CONSTRUCTOR(DeathZone)

		DeathZone(scene::Actor* actor);
		virtual ~DeathZone() = default;

	private:
		virtual void onAttach(scene::Actor* previousActor) override;
		void onTriggerEnter(scene::Actor* actor);

		uint32_t mTriggerEvent;
	};

	REGISTER_COMPONENT_INSPECTOR(DeathZone)
}

META_CLASS_PARENT(maj::script::DeathZone, maj::scene::CScript, 0u)

#endif