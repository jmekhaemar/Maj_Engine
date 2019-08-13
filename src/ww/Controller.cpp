#include "Controller.h"

#include <GLFW/glfw3.h>
#include <Logging.h>

namespace maj::ww
{
	const Controller Controller::skInvalid {};

	Controller::Controller()
		: mIndex { 0u }, mkName { "invalid" }, mKeys {}, mAxes { nullptr }, mAxisCount { 0u }
	{}

	Controller::Controller(uint8_t index)
		: mIndex { index }, mkName { glfwGetJoystickName(static_cast<int32_t>(mIndex)) }, mKeys {}, mAxes { nullptr }, mAxisCount { 0u }
	{
		int32_t count { 0u };
		glfwGetJoystickButtons(static_cast<int32_t>(mIndex), &count);
		mKeys.resize(count);

		count = 0u;
		mAxes = glfwGetJoystickAxes(static_cast<int32_t>(mIndex), &count);
		mAxisCount = static_cast<uint8_t>(count);
	}

	Controller::Controller(Controller&& other)
		: mIndex { other.mIndex }, mkName { other.mkName },mKeys { std::move(other.mKeys) }, mAxes { other.mAxes },mAxisCount { other.mAxisCount }
	{
		other.mkName = nullptr;
		other.mAxes = nullptr;
		other.mAxisCount = 0u;
	}

	Controller::~Controller()
	{}

	bool Controller::update()
	{
		if (!glfwJoystickPresent(static_cast<int32_t>(mIndex)))
			return false;

		uint8_t keyCount { static_cast<uint8_t>(mKeys.size()) };
		for (uint8_t i { 0u } ; i < keyCount ; ++i)
			mKeys[i].set(Key::EState::NONE);

		int32_t count { 0u };
		const uint8_t* kButtons { glfwGetJoystickButtons(static_cast<int32_t>(mIndex), &count) };
		for (uint8_t i { 0u } ; i < keyCount ; ++i)
			mKeys[i].set(kButtons[i] == GLFW_PRESS ? Key::EState::PRESSED : Key::EState::RELEASED);

		mAxes = glfwGetJoystickAxes(static_cast<int32_t>(mIndex), &count);

		return true;
	}

	const char* Controller::name() const
	{
		return mkName;
	}

	const Key& Controller::key(uint8_t i) const
	{
		uint8_t keyCount { static_cast<uint8_t>(mKeys.size()) };
		if (i >= keyCount)
		{
			LOG(LogWarning, "failed to get controller key: index out of range");
			return Key::skInvalid;
		}

		return mKeys[i];
	}

	float Controller::axis(uint8_t i) const
	{
		if (i >= mAxisCount || !mAxes)
		{
			LOG(LogWarning, "failed to get controller axis: index out of range");
			return 0.f;
		}

		return mAxes[i];
	}

	uint8_t Controller::keyCount() const
	{
		return static_cast<uint8_t>(mKeys.size());
	}

	uint8_t Controller::axisCount() const
	{
		return mAxisCount;
	}
}