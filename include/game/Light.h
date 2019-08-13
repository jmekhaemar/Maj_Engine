#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <unordered_map>
#include <Vec3.h>

#include <scene/CScript.h>

#include "Project.h"

namespace maj::script
{
	class Light final : public scene::CScript
	{
		ENABLE_META_PARENT(Light, scene::CScript)

	public:
		LOAD_CONSTRUCTOR_CUSTOM(Light)

		Light(scene::Actor* actor);
		virtual ~Light() = default;

		virtual void update(float deltaTime) override;

	private:
		float mTime;
	};

	REGISTER_COMPONENT_INSPECTOR(Light)
}

META_CLASS_PARENT(maj::script::Light, maj::scene::CScript, 0u)

#endif
