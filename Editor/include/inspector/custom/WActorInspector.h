#ifndef _WACTOR_INSPECTOR_H_
#define _WACTOR_INSPECTOR_H_

#include "../WInspectorComponent.h"

#include <scene/Actor.h>

namespace maj::editor
{
	class WActorInspector : public WInspectorComponent
	{
	public:
		WActorInspector(QWidget* parent, QFormLayout* layout = nullptr);

		template<typename T>
		void load(T* component);

	private:
		void onNameChanged();

		scene::Actor* mActor;
		std::string mOldName;
	};
}

#include "WActorInspector.inl"

#endif