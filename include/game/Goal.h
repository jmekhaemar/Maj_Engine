#ifndef _GOAL_H_
#define _GOAL_H_

#include <scene/CScript.h>
#include <Timer.h>

#include "Project.h"

namespace maj::script
{
	class Goal final : public scene::CScript
	{
		ENABLE_META_PARENT(Goal, scene::CScript)

	public:
		LOAD_CONSTRUCTOR(Goal)

		Goal(scene::Actor* actor);
		~Goal() = default;

		virtual void update(float deltaTime) override;

	private:
		virtual void onAttach(scene::Actor* previousActor) override;
		void onTriggerEnter(scene::Actor* actor);

		uint32_t mTriggerEvent;
		Timer mTimer;
	};

	REGISTER_COMPONENT_INSPECTOR(Goal)
}

META_CLASS_PARENT(maj::script::Goal, maj::scene::CScript, 0u)

#endif