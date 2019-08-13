#ifndef _CSCRIPT_H_
#define _CSCRIPT_H_

#include "scene/IComponent.h"
#include "scene/Actor.h"

namespace maj::scene
{
	class CScript : public IComponent
	{
		ENABLE_META_PARENT(CScript, IComponent)

	public:
		LOAD_CONSTRUCTOR(CScript)

		CScript(Actor* actor);
		virtual ~CScript();

	protected:
		CScript() = default;

		virtual void update(float deltaTime);
		virtual void lateUpdate(float deltaTime);
		virtual void fixedUpdate();

		virtual void onAttach(Actor* previousActor) override;

	private:
		uint32_t mUpdateIndex;
		uint32_t mLateUpdateIndex;
		uint32_t mFixedUpdateIndex;
	};
}

META_CLASS_PARENT(maj::scene::CScript, maj::scene::IComponent, 0u)

#endif