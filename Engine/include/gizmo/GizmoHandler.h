#ifndef _GIZMO_HANDLER_H_
#define _GIZMO_HANDLER_H_

#include <unordered_map>
#include <forward_list>

#include <Service.h>
#include <Event.h>

#include "gizmo/Selection.h"
#include "scene/IComponent.h"

namespace maj::gizmo
{
	class IGizmo;

	class GizmoHandler final : public Service
	{
	private:
		struct GizmoList final : public NonCopyable
		{
		public:
			GizmoList(GizmoHandler& handler, bool enabled);
			GizmoList(GizmoList&& other);
			~GizmoList();

			void add(IGizmo* gizmo);
			void remove(IGizmo* gizmo);

			void enable(IGizmo* gizmo, bool enabled);
			void enable(bool enabled);

		private:
			GizmoHandler& mHandler;
			std::forward_list<IGizmo*> mList;
			bool mIsEnabled;
		};

	public:
		GizmoHandler();
		~GizmoHandler() = default;

		template <typename T, typename... Args>
		T* add(scene::IComponent* component, Args&&... args);
		void remove(scene::IComponent* component);
		void remove(scene::IComponent* component, IGizmo* gizmo);

		void enable(scene::IComponent* component, bool enabled);
		void enable(scene::IComponent* component, IGizmo* gizmo, bool enabled);
		void enable(bool enabled);

		const Selection& selection() const;
		Selection& selection();

		void selectActor(scene::Actor* actor);

		const scene::Actor* selectedActor() const;
		scene::Actor* selectedActor();

		void draw() const;
		void update(float deltaTime);
		void lateUpdate(float deltaTime);
		void fixedUpdate();

		Event<void> mOnDraw;
		Event<void, float> mOnUpdate;
		Event<void, float> mOnLateUpdate;
		Event<void> mOnFixedUpdate;

		Event<void> mOnSelectionChanged;

	private:
		std::unordered_map<scene::IComponent*, GizmoList> mGizmos;
		Selection mSelection;
		scene::Actor* mSelectedActor;
		bool mIsEnabled;
	};
}

#include "gizmo/GizmoHandler.inl"

#endif