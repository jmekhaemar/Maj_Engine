#include "Mouse.h"

namespace maj::ww
{
	Mouse::Mouse()
		: mKeys {}, mPosition {}, mRelativePos {}, mScroll { 0.f }
	{
		uint8_t count { static_cast<uint8_t>(EMouseKey::COUNT) };
		for (uint8_t i { static_cast<uint8_t>(EMouseKey::FIRST) } ; i < count ; ++i)
			mKeys.emplace(static_cast<EMouseKey>(i), Key {});
	}

	void Mouse::update()
	{
		for (std::unordered_map<EMouseKey, Key>::iterator it { mKeys.begin() } ; it != mKeys.end() ; ++it)
			it->second.set(Key::EState::NONE);

		mRelativePos.mX = mRelativePos.mY = 0.f;
		mScroll = 0.f;
	}

	void Mouse::key(EMouseKey button, Key::EState keyState)
	{
		if (!mKeys.count(button))
			return;

		mKeys[button].set(keyState);
	}

	void Mouse::position(float x, float y)
	{
		mRelativePos = mPosition;
		mPosition.mX = x;
		mPosition.mY = y;

		mRelativePos -= mPosition;
	}

	void Mouse::scroll(float y)
	{
		mScroll = y;
	}

	const Key& Mouse::key(EMouseKey button) const
	{
		return mKeys.at(button);
	}

	const math::Vec2f& Mouse::position() const
	{
		return mPosition;
	}

	const math::Vec2f& Mouse::relativePosition() const
	{
		return mRelativePos;
	}

	float Mouse::scroll() const
	{
		return mScroll;
	}
}