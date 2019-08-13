#ifndef _GIZMO_HANDLER_INL_
#define _GIZMO_HANDLER_INL_

#include "gizmo/GizmoHandler.h"

#include <type_traits>

namespace maj::gizmo
{
	template <typename T, typename... Args>
	T* GizmoHandler::add(scene::IComponent* component, Args&&... args)
	{
		static_assert(std::is_base_of<IGizmo, T>::value, "failed to add gizmo: type doesn't inherit from IGizmo");

		if (mGizmos.count(component) == 0)
			mGizmos.emplace(component, GizmoList { *this, mIsEnabled });
		
		T* gizmo { new T { component, std::forward<Args&&>(args)... } };
		mGizmos.at(component).add(gizmo);

		return gizmo;
	}
}

#endif