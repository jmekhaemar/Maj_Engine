#include "Input.h"

#include <Logging.h>

#include "Window.h"

namespace maj::ww
{
	const uint8_t Input::skMaxJoystickCount { GLFW_JOYSTICK_LAST + 1u };

	constexpr EKey Input::convertKey(int32_t key)
	{
		switch (key)
		{
			case GLFW_KEY_SPACE:
			return EKey::SPACE;

			case GLFW_KEY_APOSTROPHE:
			return EKey::APOSTROPHE;

			case GLFW_KEY_COMMA:
			return EKey::COMMA;

			case GLFW_KEY_MINUS:
			return EKey::MINUS;

			case GLFW_KEY_PERIOD:
			return EKey::PERIOD;

			case GLFW_KEY_SLASH:
			return EKey::SLASH;

			case GLFW_KEY_0:
			return EKey::NUM_0;

			case GLFW_KEY_1:
			return EKey::NUM_1;

			case GLFW_KEY_2:
			return EKey::NUM_2;

			case GLFW_KEY_3:
			return EKey::NUM_3;

			case GLFW_KEY_4:
			return EKey::NUM_4;

			case GLFW_KEY_5:
			return EKey::NUM_5;

			case GLFW_KEY_6:
			return EKey::NUM_6;

			case GLFW_KEY_7:
			return EKey::NUM_7;

			case GLFW_KEY_8:
			return EKey::NUM_8;

			case GLFW_KEY_9:
			return EKey::NUM_9;

			case GLFW_KEY_SEMICOLON:
			return EKey::SEMICOLON;

			case GLFW_KEY_EQUAL:
			return EKey::EQUAL;

			case GLFW_KEY_A:
			return EKey::A;

			case GLFW_KEY_B:
			return EKey::B;

			case GLFW_KEY_C:
			return EKey::C;

			case GLFW_KEY_D:
			return EKey::D;

			case GLFW_KEY_E:
			return EKey::E;

			case GLFW_KEY_F:
			return EKey::F;

			case GLFW_KEY_G:
			return EKey::G;

			case GLFW_KEY_H:
			return EKey::H;

			case GLFW_KEY_I:
			return EKey::I;

			case GLFW_KEY_J:
			return EKey::J;

			case GLFW_KEY_K:
			return EKey::K;

			case GLFW_KEY_L:
			return EKey::L;

			case GLFW_KEY_M:
			return EKey::M;

			case GLFW_KEY_N:
			return EKey::N;

			case GLFW_KEY_O:
			return EKey::O;

			case GLFW_KEY_P:
			return EKey::P;

			case GLFW_KEY_Q:
			return EKey::Q;

			case GLFW_KEY_R:
			return EKey::R;

			case GLFW_KEY_S:
			return EKey::S;

			case GLFW_KEY_T:
			return EKey::T;

			case GLFW_KEY_U:
			return EKey::U;

			case GLFW_KEY_V:
			return EKey::V;

			case GLFW_KEY_W:
			return EKey::W;

			case GLFW_KEY_X:
			return EKey::X;

			case GLFW_KEY_Y:
			return EKey::Y;

			case GLFW_KEY_Z:
			return EKey::Z;

			case GLFW_KEY_LEFT_BRACKET:
			return EKey::LEFT_BRACKET;

			case GLFW_KEY_BACKSLASH:
			return EKey::BACKSLASH;

			case GLFW_KEY_RIGHT_BRACKET:
			return EKey::RIGHT_BRACKET;

			case GLFW_KEY_GRAVE_ACCENT:
			return EKey::GRAVE_ACCENT;

			case GLFW_KEY_ESCAPE:
			return EKey::ESCAPE;

			case GLFW_KEY_ENTER:
			return EKey::ENTER;

			case GLFW_KEY_TAB:
			return EKey::TAB;

			case GLFW_KEY_BACKSPACE:
			return EKey::BACKSPACE;

			case GLFW_KEY_INSERT:
			return EKey::INSERT;

			case GLFW_KEY_DELETE:
			return EKey::DELETE_KEY;

			case GLFW_KEY_RIGHT:
			return EKey::RIGHT;

			case GLFW_KEY_LEFT:
			return EKey::LEFT;

			case GLFW_KEY_DOWN:
			return EKey::DOWN;

			case GLFW_KEY_UP:
			return EKey::UP;

			case GLFW_KEY_PAGE_UP:
			return EKey::PAGE_UP;

			case GLFW_KEY_PAGE_DOWN:
			return EKey::PAGE_DOWN;

			case GLFW_KEY_HOME:
			return EKey::HOME;

			case GLFW_KEY_END:
			return EKey::END;

			case GLFW_KEY_CAPS_LOCK:
			return EKey::CAPS_LOCK;

			case GLFW_KEY_SCROLL_LOCK:
			return EKey::SCROLL_LOCK;

			case GLFW_KEY_NUM_LOCK:
			return EKey::NUM_LOCK;

			case GLFW_KEY_PRINT_SCREEN:
			return EKey::PRINT_SCREEN;

			case GLFW_KEY_PAUSE:
			return EKey::PAUSE;

			case GLFW_KEY_F1:
			return EKey::F1;

			case GLFW_KEY_F2:
			return EKey::F2;

			case GLFW_KEY_F3:
			return EKey::F3;

			case GLFW_KEY_F4:
			return EKey::F4;

			case GLFW_KEY_F5:
			return EKey::F5;

			case GLFW_KEY_F6:
			return EKey::F6;

			case GLFW_KEY_F7:
			return EKey::F7;

			case GLFW_KEY_F8:
			return EKey::F8;

			case GLFW_KEY_F9:
			return EKey::F9;

			case GLFW_KEY_F10:
			return EKey::F10;

			case GLFW_KEY_F11:
			return EKey::F11;

			case GLFW_KEY_F12:
			return EKey::F12;

			case GLFW_KEY_KP_0:
			return EKey::PAD_0;

			case GLFW_KEY_KP_1:
			return EKey::PAD_1;

			case GLFW_KEY_KP_2:
			return EKey::PAD_2;

			case GLFW_KEY_KP_3:
			return EKey::PAD_3;

			case GLFW_KEY_KP_4:
			return EKey::PAD_4;

			case GLFW_KEY_KP_5:
			return EKey::PAD_5;

			case GLFW_KEY_KP_6:
			return EKey::PAD_6;

			case GLFW_KEY_KP_7:
			return EKey::PAD_7;

			case GLFW_KEY_KP_8:
			return EKey::PAD_8;

			case GLFW_KEY_KP_9:
			return EKey::PAD_9;

			case GLFW_KEY_KP_DECIMAL:
			return EKey::PAD_DECIMAL;

			case GLFW_KEY_KP_DIVIDE:
			return EKey::PAD_DIVIDE;

			case GLFW_KEY_KP_MULTIPLY:
			return EKey::PAD_MULTIPLY;

			case GLFW_KEY_KP_SUBTRACT:
			return EKey::PAD_SUBTRACT;

			case GLFW_KEY_KP_ADD:
			return EKey::PAD_ADD;

			case GLFW_KEY_KP_ENTER:
			return EKey::PAD_ENTER;

			case GLFW_KEY_KP_EQUAL:
			return EKey::PAD_EQUAL;

			case GLFW_KEY_LEFT_SHIFT:
			return EKey::LEFT_SHIFT;

			case GLFW_KEY_LEFT_CONTROL:
			return EKey::LEFT_CONTROL;

			case GLFW_KEY_LEFT_ALT:
			return EKey::LEFT_ALT;

			case GLFW_KEY_RIGHT_SHIFT:
			return EKey::RIGHT_SHIFT;

			case GLFW_KEY_RIGHT_CONTROL:
			return EKey::RIGHT_CONTROL;

			case GLFW_KEY_RIGHT_ALT:
			return EKey::RIGHT_ALT;

			case GLFW_KEY_MENU:
			return EKey::MENU;

			default:
			return EKey::INVALID;
		}
	}

	constexpr EMouseKey Input::convertMouseKey(int32_t key)
	{
		switch (key)
		{
			case GLFW_MOUSE_BUTTON_LEFT:
			return EMouseKey::LEFT;

			case GLFW_MOUSE_BUTTON_RIGHT:
			return EMouseKey::RIGHT;

			case GLFW_MOUSE_BUTTON_MIDDLE:
			return EMouseKey::MIDDLE;

			case GLFW_MOUSE_BUTTON_4:
			return EMouseKey::BUTTON_4;

			case GLFW_MOUSE_BUTTON_5:
			return EMouseKey::BUTTON_5;

			default:
			return EMouseKey::INVALID;
		}
	}

	void Input::onKey(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
	{
		(void) window;
		(void) scancode;
		(void) mods;

		EKey keyValue { convertKey(key) };
		if (keyValue == EKey::INVALID)
			return;

		Service::get<Input>()->mKeys[keyValue].set(action == GLFW_RELEASE ? Key::EState::RELEASED : Key::EState::PRESSED);
	}

	void Input::onMouseKey(GLFWwindow* window, int32_t button, int32_t action, int32_t mods)
	{
		(void) window;
		(void) mods;

		EMouseKey key { convertMouseKey(button) };
		if (key == EMouseKey::INVALID)
			return;

		Service::get<Input>()->mMouse.key(key, action == GLFW_RELEASE ? Key::EState::RELEASED : Key::EState::PRESSED);
	}

	void Input::onMouseScroll(GLFWwindow* window, double x, double y)
	{
		(void) window;
		(void) x;

		Service::get<Input>()->mMouse.scroll(static_cast<float>(y));
	}

	void Input::onMousePosition(GLFWwindow* window, double x, double y)
	{
		(void) window;
		Service::get<Input>()->mMouse.position(static_cast<float>(x), static_cast<float>(y));
	}

	void Input::onControllerConfig(int32_t joystick, int32_t event)
	{
		Input* input { Service::get<Input>() };

		uint8_t index { static_cast<uint8_t>(joystick) };

		if (event == GLFW_CONNECTED)
		{
			input->mControllers.emplace(index, index);
			input->mOnControllerConnected(static_cast<uint8_t&&>(index));
		}
		else if (event == GLFW_DISCONNECTED)
		{
			if (input->mControllers.count(index) == 0u)
				return;

			input->mOnControllerDisconnected(static_cast<uint8_t&&>(index));
		}
	}

	Input::Input()
		: Service { typeid(Input).name() }, mKeys {}, mMouse {}, mControllers {}
	{
		uint8_t count { static_cast<uint8_t>(EKey::COUNT) };
		for (uint8_t i { static_cast<uint8_t>(EKey::FIRST) } ; i < count ; ++i)
			mKeys.emplace(static_cast<EKey>(i), Key {});

		for (uint8_t i { GLFW_JOYSTICK_1 } ; i < GLFW_JOYSTICK_LAST ; ++i)
		{
			if (glfwJoystickPresent(i))
				mControllers.emplace(i, i);
		}

		glfwSetJoystickCallback(&Input::onControllerConfig);
	}

	void Input::viewport(GLFWwindow* window)
	{
		if (!window)
			return;

		glfwSetKeyCallback(window, &Input::onKey);
		glfwSetMouseButtonCallback(window, &Input::onMouseKey);
		glfwSetScrollCallback(window, &Input::onMouseScroll);
		glfwSetCursorPosCallback(window, &Input::onMousePosition);
	}

	void Input::update()
	{
		for (std::unordered_map<EKey, Key>::iterator it { mKeys.begin() } ; it != mKeys.end() ; ++it)
			it->second.set(Key::EState::NONE);

		mMouse.update();

		for (std::unordered_map<uint8_t, Controller>::iterator it { mControllers.begin() } ; it != mControllers.end() ;)
		{
			if (!it->second.update())
				it = mControllers.erase(it);
			else
				++it;
		}
	}

	const Key& Input::key(EKey keyValue) const
	{
		if (keyValue == EKey::INVALID)
			return Key::skInvalid;

		return mKeys.at(keyValue);
	}

	const Mouse& Input::mouse() const
	{
		return mMouse;
	}

	uint8_t Input::controllerCount() const
	{
		return static_cast<uint8_t>(mControllers.size());
	}

	const Controller& Input::controller(uint8_t index) const
	{
		if (!glfwJoystickPresent(static_cast<int32_t>(index)) || mControllers.count(index) == 0u)
			return Controller::skInvalid;

		return mControllers.at(index);
	}
}