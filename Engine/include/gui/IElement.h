#ifndef _GUI_IELEMENT_H_
#define _GUI_IELEMENT_H_

#include <Flag.h>
#include <Event.h>
#include <ISerializable.h>
#include <Vec2.h>
#include <IDescriptor.h>

#include "Color.h"

namespace maj::gui
{
	enum class ECoordSpace
	{
		SCREEN,
		WORLD
	};

	enum class EAnchorFlag : uint8_t
	{
		CENTER = 0u,
		RIGHT = 1u,
		LEFT = 2u,
		UP = 4u,
		DOWN = 8u
	};

	enum class ECoordFlag : uint8_t
	{
		UNANCHORED = 0u,
		ANCHORED = 1u,
		NORMALIZED = 0u,
		PIXELATED = 2u
	};

	DECLARE_FLAG(EAnchorFlag)
	DECLARE_FLAG(ECoordFlag)

	class CWidget;

	class IElement : public NonCopyable, public ISerializable
	{
		ENABLE_META(IElement)

	public:
		IElement();
		IElement(CWidget* widget, EAnchorFlag internalAnchor = EAnchorFlag::UP | EAnchorFlag::LEFT, EAnchorFlag externalAnchor = EAnchorFlag::CENTER);
		virtual ~IElement();

		INITIALIZE_SIGNATURE();

		const CWidget* widget() const;
		void widget(CWidget* widget);

		virtual void draw() const = 0;
		virtual void camera(const rhi::IDescriptor* kCamera) = 0;

		EAnchorFlag internalAnchor() const;
		EAnchorFlag externalAnchor() const;
		void internalAnchor(EAnchorFlag anchorFlag);
		void externalAnchor(EAnchorFlag anchorFlag);

		const math::Vec2f& position() const;
		math::Vec2f position(ECoordFlag coordFlag) const;
		void position(const math::Vec2f& kPos, ECoordFlag coordFlag = ECoordFlag::NORMALIZED | ECoordFlag::ANCHORED);

		const math::Vec2f& size() const;
		math::Vec2f size(ECoordFlag coordFlag) const;
		void size(const math::Vec2f& kSize, ECoordFlag coordFlag = ECoordFlag::NORMALIZED);

		const Color& color() const;
		void color(const Color& kColor);

		virtual void resize(const math::Vec2f& kFactors);

		bool isEnabled() const;
		void enable(bool enabled);

		Event<void, bool> mOnEnabled;

	protected:
		virtual void onAttach();
		virtual void onSpaceChanged(ECoordSpace space) = 0;
		virtual void onSizeChanged();

		math::Vec2f normalize(const math::Vec2f& kValue) const;
		math::Vec2f pixelize(const math::Vec2f& kValue) const;
		void anchoredPosition(math::Vec2f& pos) const;
		void unanchoredPosition(math::Vec2f& pos) const;

		void reposition(math::Vec2f& pos, const math::Vec2f& kFactors) const;
		void rescale(math::Vec2f& size, const math::Vec2f& kFactors) const;

		CWidget* mWidget;
		const rhi::IDescriptor* mkCamera;

		EAnchorFlag mInternalAnchor;
		EAnchorFlag mExternalAnchor;

		math::Vec2f mPosition;
		math::Vec2f mSize;
		Color mColor;

		bool mIsEnabled;
		uint32_t mSpaceEvent;
	};
}

#include "gui/IElement.inl"

META_ENUM(maj::gui::ECoordSpace, 2u)
META_ENUM_VALUE(maj::gui::ECoordSpace, maj::gui::ECoordSpace::SCREEN)
META_ENUM_VALUE(maj::gui::ECoordSpace, maj::gui::ECoordSpace::WORLD)

META_FLAG(maj::gui::EAnchorFlag, 5u)
META_FLAG_VALUE(maj::gui::EAnchorFlag, maj::gui::EAnchorFlag::CENTER, 0u)
META_FLAG_VALUE(maj::gui::EAnchorFlag, maj::gui::EAnchorFlag::RIGHT, 1u)
META_FLAG_VALUE(maj::gui::EAnchorFlag, maj::gui::EAnchorFlag::LEFT, 2u)
META_FLAG_VALUE(maj::gui::EAnchorFlag, maj::gui::EAnchorFlag::UP, 3u)
META_FLAG_VALUE(maj::gui::EAnchorFlag, maj::gui::EAnchorFlag::DOWN, 4u)

META_CLASS(maj::gui::IElement, 6u)
META_FIELD(maj::gui::IElement, maj::gui::EAnchorFlag, mInternalAnchor, 0u)
META_FIELD(maj::gui::IElement, maj::gui::EAnchorFlag, mExternalAnchor, 1u)
META_FIELD(maj::gui::IElement, maj::math::Vec2f, mPosition, 2u)
META_FIELD(maj::gui::IElement, maj::math::Vec2f, mSize, 3u)
META_FIELD(maj::gui::IElement, maj::Color, mColor, 4u)
META_FIELD(maj::gui::IElement, bool, mIsEnabled, 5u)

#endif