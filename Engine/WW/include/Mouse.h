#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <unordered_map>

#include <Vec2.h>

#include "Key.h"

namespace maj::ww
{
	enum class EMouseKey : uint8_t
	{
		INVALID = 0u,
		LEFT,
		RIGHT,
		MIDDLE,
		BUTTON_4,
		BUTTON_5,
		FIRST = LEFT,
		LAST = BUTTON_5,
		COUNT = LAST - FIRST + 1u
	};

	class Mouse final : public NonCopyable
	{
	public:
		Mouse();
		~Mouse() = default;

		void update();

		void key(EMouseKey button, Key::EState keyState);
		void position(float x, float y);
		void scroll(float y);

		const Key& key(EMouseKey button) const;
		const math::Vec2f& position() const;
		const math::Vec2f& relativePosition() const;
		float scroll() const;

	private:
		std::unordered_map<EMouseKey, Key> mKeys;

		math::Vec2f mPosition;
		math::Vec2f mRelativePos;
		float mScroll;
	};
}

#endif