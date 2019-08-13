#ifndef _CWIDGET_H_
#define _CWIDGET_H_

#include <Buffer.h>

#include "scene/IComponent.h"
#include "gui/IElement.h"

namespace maj::gui
{
	class CWidget final : public scene::IComponent
	{
		ENABLE_META_PARENT(CWidget, scene::IComponent)

	public:
		LOAD_CONSTRUCTOR_CUSTOM(CWidget)

		CWidget(scene::Actor* actor, ECoordSpace space = ECoordSpace::SCREEN);
		virtual ~CWidget();
		
		ECoordSpace space() const;
		void space(ECoordSpace space);

		const math::Vec2ui& reference() const;
		void reference(const math::Vec2ui& kRef);

		template <typename T>
		T* element(uint32_t index);
		template <typename T>
		const T* element(uint32_t index) const;

		template <typename T, typename... Args>
		T* addElement(Args&&... args);
		void removeElements();
		void removeElement(uint32_t index);
		void removeElement(const IElement* kElement);

		const rhi::Buffer::SubBuffer* buffer() const;

		virtual void awake() override;

		Event<void, ECoordSpace> mOnSpaceChanged;

	private:
		virtual void onAttach(scene::Actor* previousActor) override;

		void draw() const;

		ECoordSpace mSpace;
		math::Vec2ui mRefScreen;
		std::vector<IElement*> mElements;
		rhi::Buffer::SubBuffer* mBuffer;

		uint32_t mDrawEventIndex;
		uint32_t mCameraEventIndex;
		uint32_t mResizeEventIndex;
	};
}

#include "gui/CWidget.inl"

META_CLASS_PARENT(maj::gui::CWidget, maj::scene::IComponent, 3u)
META_FIELD(maj::gui::CWidget, maj::gui::ECoordSpace, mSpace, 0u)
META_FIELD(maj::gui::CWidget, maj::math::Vec2ui, mRefScreen, 1u)
META_FIELD_ARRAY(maj::gui::CWidget, std::vector<maj::gui::IElement*>, mElements, 2u)

#endif