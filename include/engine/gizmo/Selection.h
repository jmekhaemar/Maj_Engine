#ifndef _SELECTION_H_
#define _SELECTION_H_

#include <unordered_map>
#include <set>

#include "gizmo/ObjectPicker.h"
#include "gizmo/Selector.h"

namespace maj::gizmo
{
	class Selection : public NonCopyable
	{
	public:
		Selection();
		~Selection();

		uint32_t add(const rhi::MaterialInstance* kMaterialInstance, const rhi::DrawCall::Geometry& kGeometry);
		void remove(uint32_t id);

		const Selector* selector(uint32_t id) const;
		Selector* selector(uint32_t id);

		void pick(const math::Vec2f& kPos);
		void clear();

		const Selector* selected() const;
		Selector* selected();

	private:
		ObjectPicker mObjectPicker;
		std::unordered_map<uint32_t, Selector> mSelectors;
		uint32_t mSelectedId;
		bool mCleared;
	};
}

#endif