#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <scene/CScript.h>
#include <scene/CSkeletalMesh.h>

#include "Project.h"

namespace maj::script
{
	class Player final : public scene::CScript
	{
		ENABLE_META_PARENT(Player, scene::CScript)

	public:
		LOAD_CONSTRUCTOR_CUSTOM(Player)

		Player(scene::Actor* actor);
		virtual ~Player() = default;

		virtual void awake() override;

		virtual void update(float deltaTime) override;
		virtual void fixedUpdate() override;

	private:
		scene::CSkeletalMesh* mMesh;
		bool mIsWalking;

		float mSpeed;
		float mAngularSpeed;
		float mJumpHeight;

		bool mJump;
		bool mIsInAir;

		math::Vec3f mDeltaPos;
		math::Vec3f mEuler;
	};

	REGISTER_COMPONENT_INSPECTOR(Player)
}

META_CLASS_PARENT(maj::script::Player, maj::scene::CScript, 3u)
META_FIELD(maj::script::Player, float, mSpeed, 0u)
META_FIELD(maj::script::Player, float, mAngularSpeed, 1u)
META_FIELD(maj::script::Player, float, mJumpHeight, 2u)

#endif
