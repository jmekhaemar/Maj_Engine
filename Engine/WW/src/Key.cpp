#include "Key.h"

namespace maj::ww
{
	const Key Key::skInvalid {};

	Key::Key()
		: mPressed { false }, mOnce { false }
	{}

	Key::Key(Key&& other)
		: mPressed { other.mPressed }, mOnce { other.mOnce }
	{}

	void Key::set(EState keyState)
	{
		if (keyState == EState::PRESSED)
		{
			mOnce = !mPressed;
			mPressed = true;
		}
		else if (keyState == EState::RELEASED)
		{
			mOnce = mPressed;
			mPressed = false;
		}
		else
			mOnce = false;
	}

	bool Key::isPressed() const
	{
		return mPressed && mOnce;
	}

	bool Key::isReleased() const
	{
		return !mPressed && mOnce;
	}

	bool Key::isDown() const
	{
		return mPressed;
	}

	bool Key::isUp() const
	{
		return !mPressed;
	}
}