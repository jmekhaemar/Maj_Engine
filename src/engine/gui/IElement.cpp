#include "gui/IElement.h"

#include "gui/CWidget.h"

namespace maj::gui
{
	DEFINE_HASH_OVERLOAD(IElement)

		IElement::IElement()
		: mWidget { nullptr }, mInternalAnchor { EAnchorFlag::UP | EAnchorFlag::LEFT }, mExternalAnchor { EAnchorFlag::CENTER }, mPosition { 0.5f }, mSize {}, mColor { },
		mIsEnabled { true }, mSpaceEvent { 0u }
	{}

	IElement::IElement(CWidget* widget, EAnchorFlag internalAnchor, EAnchorFlag externalAnchor)
		: mWidget { widget }, mInternalAnchor { internalAnchor }, mExternalAnchor { externalAnchor }, mPosition {}, mSize {}, mColor { }, mIsEnabled { true }, mSpaceEvent { 0u }
	{
		if (!mWidget)
			THROW("failed to create gui element: no widget")

		// Fixing position according to anchor
		if ((mExternalAnchor & EAnchorFlag::LEFT) != EAnchorFlag::LEFT)
			mPosition.mX = (mExternalAnchor & EAnchorFlag::RIGHT) == EAnchorFlag::RIGHT ? 1.f : 0.5f;

		if ((mExternalAnchor & EAnchorFlag::UP) != EAnchorFlag::UP)
			mPosition.mY = (mExternalAnchor & EAnchorFlag::DOWN) == EAnchorFlag::DOWN ? 1.f : 0.5f;

		initialize();
	}

	IElement::~IElement()
	{
		if (mWidget)
			mWidget->mOnSpaceChanged -= mSpaceEvent;
	}

	void IElement::initialize()
	{}

	void IElement::widget(CWidget* widget)
	{
		if (mWidget == widget)
			return;

		mWidget = widget;
		onAttach();
		enable(mWidget->isEnabled());
	}

	EAnchorFlag IElement::internalAnchor() const
	{
		return mInternalAnchor;
	}

	EAnchorFlag IElement::externalAnchor() const
	{
		return mExternalAnchor;
	}

	void IElement::internalAnchor(EAnchorFlag anchorFlag)
	{
		mInternalAnchor = anchorFlag;
	}
	
	void IElement::externalAnchor(EAnchorFlag anchorFlag)
	{
		mExternalAnchor = anchorFlag;
	}

	const math::Vec2f& IElement::position() const
	{
		return mPosition;
	}

	math::Vec2f IElement::position(ECoordFlag coordFlag) const
	{
		math::Vec2f pos { mPosition };
		
		if ((coordFlag & ECoordFlag::ANCHORED) == ECoordFlag::ANCHORED)
			anchoredPosition(pos);

		return (coordFlag & ECoordFlag::PIXELATED) == ECoordFlag::PIXELATED ? pixelize(pos) : pos;
	}

	void IElement::position(const math::Vec2f& kPos, ECoordFlag coordFlag)
	{
		mPosition = (coordFlag & ECoordFlag::PIXELATED) == ECoordFlag::PIXELATED ? normalize(kPos) : kPos;

		if ((coordFlag & ECoordFlag::ANCHORED) == ECoordFlag::ANCHORED)
			unanchoredPosition(mPosition);
	}

	const math::Vec2f& IElement::size() const
	{
		return mSize;
	}

	math::Vec2f IElement::size(ECoordFlag coordFlag) const
	{
		if ((coordFlag & ECoordFlag::PIXELATED) == ECoordFlag::PIXELATED)
			return pixelize(mSize);

		return mSize;
	}

	void IElement::size(const math::Vec2f& kSize, ECoordFlag coordFlag)
	{
		const math::Vec2f kAnchoredPos { position(ECoordFlag::NORMALIZED | ECoordFlag::ANCHORED) };
		
		mSize = (coordFlag & ECoordFlag::PIXELATED) == ECoordFlag::PIXELATED ? normalize(kSize) : kSize;

		// Fixing position according to anchor
		position(kAnchoredPos, ECoordFlag::NORMALIZED | ECoordFlag::ANCHORED);

		onSizeChanged();
	}

	const Color& IElement::color() const
	{
		return mColor;
	}

	void IElement::color(const Color& kColor)
	{
		mColor = kColor;
	}

	void IElement::resize(const math::Vec2f& kFactors)
	{
		math::Vec2f pos { position(ECoordFlag::NORMALIZED | ECoordFlag::ANCHORED) };

		// Scaling position according to external anchor
		reposition(pos, kFactors);

		// Scaling size according to external anchor
		rescale(mSize, kFactors);

		// Fixing position according to internal anchor
		position(pos, ECoordFlag::NORMALIZED | ECoordFlag::ANCHORED);

		onSizeChanged();
	}

	bool IElement::isEnabled() const
	{
		return mIsEnabled;
	}

	void IElement::enable(bool enabled)
	{
		if (mIsEnabled == enabled)
			return;

		mIsEnabled = enabled;
		mOnEnabled(static_cast<bool&&>(mIsEnabled));
	}

	void IElement::onAttach()
	{
		mSpaceEvent = mWidget->mOnSpaceChanged + std::bind(&IElement::onSpaceChanged, this, std::placeholders::_1);
	}

	void IElement::onSizeChanged()
	{}

	math::Vec2f IElement::normalize(const math::Vec2f& kValue) const
	{
		const math::Vec2ui& kRef { mWidget->reference() };
		return { kValue.mX / static_cast<float>(kRef.mX), kValue.mY / static_cast<float>(kRef.mY) };
	}

	math::Vec2f IElement::pixelize(const math::Vec2f& kValue) const
	{
		const math::Vec2ui& kRef { mWidget->reference() };
		return { kValue.mX * static_cast<float>(kRef.mX), kValue.mY * static_cast<float>(kRef.mY) };
	}

	void IElement::anchoredPosition(math::Vec2f& pos) const
	{
		if ((mInternalAnchor & EAnchorFlag::RIGHT) == EAnchorFlag::RIGHT)
			pos.mX += mSize.mX;
		else if ((mInternalAnchor & EAnchorFlag::LEFT) != EAnchorFlag::LEFT)
			pos.mX += mSize.mX * 0.5f;

		if ((mInternalAnchor & EAnchorFlag::DOWN) == EAnchorFlag::DOWN)
			pos.mY += mSize.mY;
		else if ((mInternalAnchor & EAnchorFlag::UP) != EAnchorFlag::UP)
			pos.mY += mSize.mY * 0.5f;
	}

	void IElement::unanchoredPosition(math::Vec2f& pos) const
	{
		if ((mInternalAnchor & EAnchorFlag::RIGHT) == EAnchorFlag::RIGHT)
			pos.mX -= mSize.mX;
		else if ((mInternalAnchor & EAnchorFlag::LEFT) != EAnchorFlag::LEFT)
			pos.mX -= mSize.mX * 0.5f;

		if ((mInternalAnchor & EAnchorFlag::DOWN) == EAnchorFlag::DOWN)
			pos.mY -= mSize.mY;
		else if ((mInternalAnchor & EAnchorFlag::UP) != EAnchorFlag::UP)
			pos.mY -= mSize.mY * 0.5f;
	}

	void IElement::reposition(math::Vec2f& pos, const math::Vec2f& kFactors) const
	{
		bool anchoredA { (mExternalAnchor & EAnchorFlag::RIGHT) == EAnchorFlag::RIGHT };
		bool anchoredB { (mExternalAnchor & EAnchorFlag::LEFT) == EAnchorFlag::LEFT };
		if (anchoredA != anchoredB)
		{
			if (anchoredA)
				pos.mX = 1.f - (1.f - pos.mX) * kFactors.mX;
			else
				pos.mX *= kFactors.mX;
		}

		anchoredA = (mExternalAnchor & EAnchorFlag::DOWN) == EAnchorFlag::DOWN;
		anchoredB = (mExternalAnchor & EAnchorFlag::UP) == EAnchorFlag::UP;
		if (anchoredA != anchoredB)
		{
			if (anchoredA)
				pos.mY = 1.f - (1.f - pos.mY) * kFactors.mY;
			else
				pos.mY *= kFactors.mY;
		}
	}

	void IElement::rescale(math::Vec2f& size, const math::Vec2f& kFactors) const
	{
		EAnchorFlag scaleFlag { EAnchorFlag::RIGHT | EAnchorFlag::LEFT };
		if ((mExternalAnchor & scaleFlag) != scaleFlag)
			size.mX *= kFactors.mX;

		scaleFlag = EAnchorFlag::UP | EAnchorFlag::DOWN;
		if ((mExternalAnchor & scaleFlag) != scaleFlag)
			size.mY *= kFactors.mY;
	}
}