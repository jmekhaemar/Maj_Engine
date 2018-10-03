#include "gizmo/Selection.h"

#include "gizmo/GizmoHandler.h"

namespace maj::gizmo
{
	Selection::Selection()
		: mObjectPicker {}, mSelectors {}, mSelectedId { 0u }, mCleared { false }
	{}

	Selection::~Selection()
	{
		remove(mSelectedId);
	}

	uint32_t Selection::add(const rhi::MaterialInstance* kMaterialInstance, const rhi::DrawCall::Geometry& kGeometry)
	{
		static uint32_t sMaxCurrentId { 1u };
		uint32_t freeId { 0u };
		for (uint32_t i { 1u } ; i < sMaxCurrentId && freeId == 0u ; ++i)
		{
			if (mSelectors.count(i) != 0u)
				continue;

			freeId = i;
		}

		if (freeId == 0u)
			freeId = sMaxCurrentId++;

		mSelectors.try_emplace(freeId, freeId, kMaterialInstance, mObjectPicker.renderPass(), kGeometry);
		return freeId;
	}
	
	void Selection::remove(uint32_t id)
	{
		if (id == 0u || mSelectors.count(id) == 0u)
			return;

		if (mSelectedId == id)
		{
			mSelectors.at(mSelectedId).mOnSelected(false);
			mSelectedId = 0u;
		}

		mSelectors.erase(id);
	}

	const Selector* Selection::selector(uint32_t id) const
	{
		if (id == 0u || mSelectors.count(id) == 0u)
			return nullptr;

		return &mSelectors.at(id);
	}

	Selector* Selection::selector(uint32_t id)
	{
		if (id == 0u || mSelectors.count(id) == 0u)
			return nullptr;

		return &mSelectors.at(id);
	}

	void Selection::pick(const math::Vec2f& kPos)
	{
		math::Vec2ui screenPos { static_cast<uint32_t>(kPos.mX), static_cast<uint32_t>(kPos.mY) };
	
		uint32_t id { mObjectPicker.pick(screenPos) };
		if (mSelectedId != 0u && mSelectors.count(mSelectedId) != 0u)
		{
			if (mSelectedId == id)
				return;
			
			mSelectors.at(mSelectedId).mOnSelected(false);
		}

		mSelectedId = 0u;

		if (id != 0u && mSelectors.count(id) != 0u)
		{
			mSelectedId = id;
			mSelectors.at(mSelectedId).mOnSelected(true);
		}
		else if (id == 0u && mCleared)
		{
			mCleared = false;
			Service::get<gizmo::GizmoHandler>()->selectActor(nullptr);
		}
	}

	void Selection::clear()
	{
		if (mCleared)
			return;

		mCleared = true;
		mSelectedId = 0u;
	}

	const Selector* Selection::selected() const
	{
		if (mSelectedId == 0u || mSelectors.count(mSelectedId) == 0u)
			return nullptr;

		return &mSelectors.at(mSelectedId);
	}

	Selector* Selection::selected()
	{
		if (mSelectedId == 0u || mSelectors.count(mSelectedId) == 0u)
			return nullptr;

		return &mSelectors.at(mSelectedId);
	}
}