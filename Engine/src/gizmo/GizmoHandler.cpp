#include "gizmo/GizmoHandler.h"

#include "Engine.h"
#include "gizmo/IGizmo.h"

namespace maj::gizmo
{
	GizmoHandler::GizmoList::GizmoList(GizmoHandler& handler, bool enabled)
		: mHandler { handler }, mList {}, mIsEnabled { enabled }
	{}

	GizmoHandler::GizmoList::GizmoList(GizmoList&& other)
		: mHandler { other.mHandler }, mList { std::move(other.mList) }, mIsEnabled { other.mIsEnabled }
	{}

	GizmoHandler::GizmoList::~GizmoList()
	{
		for (IGizmo* gizmo : mList)
		{
			if (mIsEnabled)
				gizmo->enable(mHandler, false);

			delete gizmo;
		}
	}

	void GizmoHandler::GizmoList::add(IGizmo* gizmo)
	{
		if (!gizmo)
			THROW("failed to add gizmo: no gizmo")

		mList.emplace_front(gizmo);

		if (mIsEnabled)
			gizmo->enable(mHandler, true);
	}

	void GizmoHandler::GizmoList::remove(IGizmo* gizmo)
	{
		if (!gizmo)
			return;

		std::forward_list<IGizmo*>::iterator prev { mList.before_begin() };
		for (std::forward_list<IGizmo*>::iterator it { mList.begin() } ; it != mList.end() ; ++it, ++prev)
		{
			if (*it != gizmo)
				continue;

			if (mIsEnabled)
				gizmo->enable(mHandler, false);

			delete gizmo;
			mList.erase_after(prev);
			return;
		}
	}

	void GizmoHandler::GizmoList::enable(IGizmo* gizmo, bool enabled)
	{
		if (!gizmo)
			return;

		std::forward_list<IGizmo*>::iterator prev { mList.before_begin() };
		for (std::forward_list<IGizmo*>::iterator it { mList.begin() } ; it != mList.end() ; ++it, ++prev)
		{
			if (*it != gizmo)
				continue;

			gizmo->enable(mHandler, mIsEnabled & enabled);
			return;
		}
	}

	void GizmoHandler::GizmoList::enable(bool enabled)
	{
		if (mIsEnabled == enabled)
			return;

		mIsEnabled = enabled;

		for (IGizmo* gizmo : mList)
			gizmo->enable(mHandler, mIsEnabled);
	}

	GizmoHandler::GizmoHandler()
		: Service { typeid(GizmoHandler).name() }, mGizmos {}, mSelection {}, mSelectedActor { nullptr }, mIsEnabled { true }
	{
#ifdef MAJ_EDITOR
		Engine* engine { Service::get<Engine>() };
		engine->mOnStartSimulation += [this] () -> void
		{
			enable(false);
		};
		engine->mOnStopSimulation += [this] () -> void
		{
			enable(true);
		};
#endif

		mOnUpdate += [this] (float deltaTime) -> void
		{
			(void) deltaTime;

			const ww::Input* kInput { Service::get<ww::Input>() };
			if (kInput->mouse().key(ww::EMouseKey::LEFT).isPressed())
				mSelection.pick(kInput->mouse().position());
		};
	}

	void GizmoHandler::remove(scene::IComponent* component)
	{
		if (mGizmos.count(component) == 0u)
			return;

		mGizmos.erase(component);
	}

	void GizmoHandler::remove(scene::IComponent* component, IGizmo* gizmo)
	{
		if (mGizmos.count(component) == 0u)
			return;

		mGizmos.at(component).remove(gizmo);
	}

	void GizmoHandler::enable(scene::IComponent* component, bool enabled)
	{
		if (mGizmos.count(component) == 0u)
		{
			mGizmos.emplace(component, GizmoList { *this, enabled });
			return;
		}

		mGizmos.at(component).enable(mIsEnabled && enabled);
	}

	void GizmoHandler::enable(scene::IComponent* component, IGizmo* gizmo, bool enabled)
	{
		if (mGizmos.count(component) == 0u)
			return;

		mGizmos.at(component).enable(gizmo, mIsEnabled && enabled);
	}

	void GizmoHandler::enable(bool enabled)
	{
		if (mIsEnabled == enabled)
			return;

		mIsEnabled = enabled;

		for (std::pair<scene::IComponent* const, GizmoList>& pair : mGizmos)
			pair.second.enable(!mIsEnabled ? false : pair.first->isEnabled());
	}

	const Selection& GizmoHandler::selection() const
	{
		return mSelection;
	}

	Selection& GizmoHandler::selection()
	{
		return mSelection;
	}

	void GizmoHandler::selectActor(scene::Actor* actor)
	{
		if (mSelectedActor == actor)
			return;

#ifdef MAJ_EDITOR
		scene::Actor* prevActor { mSelectedActor };
		if (prevActor)
			prevActor->select(false);
#endif

		mSelectedActor = actor;

#ifdef MAJ_EDITOR
		if (mSelectedActor)
			mSelectedActor->select(true);
#endif

		mOnSelectionChanged();
	}

	const scene::Actor* GizmoHandler::selectedActor() const
	{
		return mSelectedActor;
	}

	scene::Actor* GizmoHandler::selectedActor()
	{
		return mSelectedActor;
	}

	void GizmoHandler::draw() const
	{
		if (mIsEnabled)
			mOnDraw();
	}

	void GizmoHandler::update(float deltaTime)
	{
		if (mIsEnabled)
			mOnUpdate(static_cast<float&&>(deltaTime));
	}

	void GizmoHandler::lateUpdate(float deltaTime)
	{
		if (mIsEnabled)
			mOnLateUpdate(static_cast<float&&>(deltaTime));
	}

	void GizmoHandler::fixedUpdate()
	{
		if (mIsEnabled)
			mOnFixedUpdate();
	}
}