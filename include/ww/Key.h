#ifndef _KEY_H_
#define _KEY_H_

#include <NonCopyable.h>
#include <cstdint>

namespace maj::ww
{
	enum class EKey : uint8_t
	{
		INVALID = 0u,
		SPACE,
		APOSTROPHE,
		COMMA,
		MINUS,
		PERIOD,
		SLASH,
		NUM_0,
		NUM_1,
		NUM_2,
		NUM_3,
		NUM_4,
		NUM_5,
		NUM_6,
		NUM_7,
		NUM_8,
		NUM_9,
		SEMICOLON,
		EQUAL,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		LEFT_BRACKET,
		BACKSLASH,
		RIGHT_BRACKET,
		GRAVE_ACCENT,
		ESCAPE,
		ENTER,
		TAB,
		BACKSPACE,
		INSERT,
		DELETE_KEY,
		RIGHT,
		LEFT,
		DOWN,
		UP,
		PAGE_UP,
		PAGE_DOWN,
		HOME,
		END,
		CAPS_LOCK,
		SCROLL_LOCK,
		NUM_LOCK,
		PRINT_SCREEN,
		PAUSE,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		PAD_0,
		PAD_1,
		PAD_2,
		PAD_3,
		PAD_4,
		PAD_5,
		PAD_6,
		PAD_7,
		PAD_8,
		PAD_9,
		PAD_DECIMAL,
		PAD_DIVIDE,
		PAD_MULTIPLY,
		PAD_SUBTRACT,
		PAD_ADD,
		PAD_ENTER,
		PAD_EQUAL,
		LEFT_SHIFT,
		LEFT_CONTROL,
		LEFT_ALT,
		RIGHT_SHIFT,
		RIGHT_CONTROL,
		RIGHT_ALT,
		MENU,
		FIRST = SPACE,
		LAST = MENU,
		COUNT = LAST - FIRST + 1u
	};

	class Key final : public NonCopyable
	{
	public:
		enum class EState
		{
			NONE,
			PRESSED,
			RELEASED
		};

		static const Key skInvalid;

	public:
		Key();
		Key(Key&& other);
		~Key() = default;

		void set(EState keyState);

		// Returns true if the key was pressed in this frame
		bool isPressed() const;
		// Returns true if the key was released in this frame
		bool isReleased() const;

		// Returns true if the key is being pressed
		bool isDown() const;
		// Returns true if the key is not being pressed
		bool isUp() const;

	private:
		bool mPressed;
		bool mOnce;
	};
}

#endif