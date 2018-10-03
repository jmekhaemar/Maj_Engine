#ifndef _INPUT_H_
#define _INPUT_H_

#include <unordered_map>
#include <GLFW/glfw3.h>

#include <Service.h>
#include <Event.h>

#include "Key.h"
#include "Mouse.h"
#include "Controller.h"

namespace maj::ww
{
	class Input final : public Service
	{
	private:
		static constexpr EKey convertKey(int32_t key);
		static constexpr EMouseKey convertMouseKey(int32_t key);

		static void onKey(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
		static void onMouseKey(GLFWwindow* window, int32_t button, int32_t action, int32_t mods);
		static void onMouseScroll(GLFWwindow* window, double x, double y);
		static void onMousePosition(GLFWwindow* window, double x, double y);
		static void onControllerConfig(int32_t joystick, int32_t event);

	public:
		static const uint8_t skMaxJoystickCount;

	public:
		Input();
		~Input() = default;

		void viewport(GLFWwindow* window);
		void update();

		const Key& key(EKey keyValue) const;
		const Mouse& mouse() const;
		uint8_t controllerCount() const;
		const Controller& controller(uint8_t index) const;

		Event<void, uint8_t> mOnControllerConnected;
		Event<void, uint8_t> mOnControllerDisconnected;

	private:
		std::unordered_map<EKey, Key> mKeys;
		Mouse mMouse;
		std::unordered_map<uint8_t, Controller> mControllers;
	};
}

#endif