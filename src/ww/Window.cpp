#include "Window.h"

#include <string>
#include <limits>

#ifdef max
#undef max
#endif

#include <Instance.h>
#include <Logging.h>

namespace maj::ww
{
	const uint8_t Window::skMaxViewportCount { 4u };

	Window::Context::Context()
	{
		glfwSetErrorCallback(&Window::onError);

		if (glfwInit() == GLFW_FALSE)
			THROW("failed to create window context: failed to init glfw")
	}

	Window::Context::~Context()
	{
		glfwTerminate();
	}

	void Window::onError(int32_t error, const char* kDescription)
	{
		(void) error;
		LOG(LogError, "glfw: " << kDescription)
	}

	void Window::onViewportResized(GLFWwindow* window, int32_t width, int32_t height)
	{
		if (width == 0 || height == 0)
			return;

		Window* win { Service::get<Window>() };

		for (std::pair<const uint8_t, Viewport>& pair : win->mViewports)
		{
			if (pair.second.window() == window)
			{
				pair.second.resize(pair.first, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
				return;
			}
		}
	}

	void Window::onViewportFocused(GLFWwindow* window, int32_t focus)
	{
		Window* win { Service::get<Window>() };

		uint8_t count { static_cast<uint8_t>(win->mViewports.size()) };
		if (count == 0u)
			return;

		if (win->mViewports.count(win->mFocusViewport) != 0u)
		{
			Viewport& focusedViewport { win->mViewports.at(win->mFocusViewport) };

			bool sameWindow { focusedViewport.window() == window };
			if (sameWindow != static_cast<bool>(focus))
				focusedViewport.loseFocus(win->mFocusViewport);
		}

		if (!focus)
			return;

		for (std::pair<const uint8_t, Viewport>& pair : win->mViewports)
		{
			if (pair.second.window() == window)
			{
				pair.second.focus(pair.first);
				return;
			}
		}
	}

	void Window::onViewportClosed(GLFWwindow* window)
	{
		Window* win { Service::get<Window>() };

		uint8_t count { static_cast<uint8_t>(win->mViewports.size()) };
		if (count <= 1u)
			return;

		for (std::pair<const uint8_t, Viewport>& pair : win->mViewports)
		{
			if (pair.second.window() != window)
				continue;

			if (pair.first == 0u)
				return;

			pair.second.close(pair.first);
			win->mViewports.erase(pair.first);
			return;
		}
	}

	Window::Window(const Viewport::Info& kInfo)
		: Service { typeid(Window).name() }, mContext {}, mViewports {}, mFocusViewport { 0u }, mInput {}
	{
		mViewports.try_emplace(static_cast<uint8_t>(0u), kInfo);

		mViewports.at(0u).mOnLostFocus += [this] (uint8_t id) -> void
		{
			(void) id;
			if (mFocusViewport == 0u)
				mFocusViewport = skMaxViewportCount;
		};
		mViewports.at(0u).mOnFocus += [this] (uint8_t id) -> void
		{
			(void) id;
			mFocusViewport = 0u;
		};

		GLFWwindow* window { mViewports.at(0u).window() };
		glfwSetWindowSizeCallback(window, &Window::onViewportResized);
		glfwSetWindowFocusCallback(window, &Window::onViewportFocused);
		glfwSetWindowCloseCallback(window, &Window::onViewportClosed);

		mInput.viewport(window);
	}

	bool Window::isAnyOpen() const
	{
		// If there is no viewport, it should close
		uint8_t count { static_cast<uint8_t>(mViewports.size()) };
		if (count == 0u)
			return false;

		// Is the first window is not open, it should close
		if (!mViewports.at(0u).isOpen())
			return false;

		for (const std::pair<const uint8_t, Viewport>& kPair : mViewports)
		{
			if (kPair.second.isOpen())
				return true;
		}

		return count == 1u;
	}

	void Window::pollEvents()
	{
		mInput.update();
		glfwPollEvents();
	}

	void Window::focusViewport(uint8_t i)
	{
		if (mViewports.count(i) == 0u)
		{
			LOG(LogWarning, "failed to focus viewport: unknown index");
			return;
		}

		glfwFocusWindow(mViewports.at(i).window());
	}

	bool Window::addViewport(Viewport::Info&& info)
	{
		// Checking if exceeded max viewport
		uint8_t count { static_cast<uint8_t>(mViewports.size()) };
		if (count >= skMaxViewportCount)
		{
			LOG(LogWarning, "failed to add viewport: max viewport count reached")
			return false;
		}

		// Finding free index
		uint8_t index { skMaxViewportCount };
		for (uint8_t i { 0u } ; i < count && index == skMaxViewportCount ; ++i)
		{
			if (mViewports.count(i) == 0u)
				index = i;
		}

		if (index == skMaxViewportCount)
			index = count;

		// Creating the viewport
		mViewports.emplace(index, std::move(info));

		mViewports.at(index).mOnLostFocus += [this] (uint8_t id) -> void
		{
			if (mFocusViewport == id)
				mFocusViewport = skMaxViewportCount;
		};
		mViewports.at(index).mOnFocus += [this] (uint8_t id) -> void
		{
			mFocusViewport = id;
		};

		GLFWwindow* window { mViewports.at(index).window() };
		glfwSetWindowSizeCallback(window, &Window::onViewportResized);
		glfwSetWindowFocusCallback(window, &Window::onViewportFocused);
		glfwSetWindowCloseCallback(window, &Window::onViewportClosed);

		mInput.viewport(window);

		// Creating the surface
		try
		{
			Service::get<rhi::Instance>()->addSurface(index);
		}
		catch (const std::exception& e)
		{
			LOG(LogWarning, "failed to add viewport: failed to add surface: " << e.what());

			mViewports.at(index).close(index);
			mViewports.erase(index);
			return false;
		}

		// Focusing on the new viewport
		mFocusViewport = index;

		mOnViewportAdded(static_cast<uint8_t&&>(index), mViewports.at(index));
		return true;
	}

	void Window::removeViewport(uint8_t i)
	{
		// Checking the index
		if (mViewports.count(i) == 0u)
		{
			LOG(LogWarning, "failed to remove viewport: unknown index");
			return;
		}

		mViewports.at(i).close(i);

		// Removing the first removes all the others
		if (i == 0u)
		{
			glfwSetWindowShouldClose(mViewports.at(0u).window(), GLFW_TRUE);
			return;
		}

		mViewports.erase(i);

		if (mFocusViewport > i)
			--mFocusViewport;
	}

	uint8_t Window::viewportCount() const
	{
		return static_cast<uint8_t>(mViewports.size());
	}

	const std::unordered_map<uint8_t, Viewport>& Window::viewports() const
	{
		return mViewports;
	}

	uint8_t Window::focusViewport() const
	{
		return mFocusViewport;
	}

	const Viewport& Window::viewport(uint8_t i) const
	{
		if (mViewports.count(i) == 0u)
		{
			LOG(LogWarning, "failed to retrieve viewport: unknown index");
			return mViewports.at(0u);
		}

		return mViewports.at(i);
	}

	Viewport& Window::viewport(uint8_t i)
	{
		if (mViewports.count(i) == 0u)
		{
			LOG(LogWarning, "failed to retrieve viewport: unknown index");
			return mViewports.at(0u);
		}

		return mViewports.at(i);
	}

	const char** Window::extensionsRequired(uint32_t& count) const
	{
		return glfwGetRequiredInstanceExtensions(&count);
	}
}
