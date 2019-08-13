#include "Viewport.h"

namespace maj::ww
{
	DEFINE_HASH_OVERLOAD(Viewport::Info)

	Viewport::Info::Info(const math::Vec2ui& kExtent, const char* kTitle)
		: mExtent { kExtent }, mTitle { kTitle }
	{}

	Viewport::Info::Info(const Info& kOther)
		: mExtent { kOther.mExtent }, mTitle { kOther.mTitle }
	{}

	void Viewport::Info::initialize()
	{}

	Viewport::Info& Viewport::Info::operator=(const Info& kOther)
	{
		mExtent = kOther.mExtent;
		mTitle = kOther.mTitle;

		return *this;
	}

	Viewport::Viewport(const Info& kInfo)
		: mInfo { kInfo }, mWindow { nullptr }, mCursorMode { Cursor::EMode::FREE }
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		mWindow = glfwCreateWindow(mInfo.mExtent.mX, mInfo.mExtent.mY, mInfo.mTitle.c_str(), nullptr, nullptr);

		if (!mWindow)
			THROW("failed to create glfw window")
	}

	Viewport::Viewport(Viewport&& other)
		: mOnResized { std::move(other.mOnResized) }, mOnFocus { std::move(other.mOnFocus) }, mOnLostFocus { std::move(other.mOnLostFocus) },
		mOnClosed { std::move(other.mOnClosed) }, mInfo { std::move(other.mInfo) }, mWindow { other.mWindow }, mCursorMode { other.mCursorMode }
	{
		other.mWindow = nullptr;
	}

	Viewport::~Viewport()
	{
		if (mWindow)
			glfwDestroyWindow(mWindow);
	}

	const math::Vec2ui& Viewport::extent() const
	{
		return mInfo.mExtent;
	}

	math::Vec2f Viewport::center() const
	{
		return { static_cast<float>(mInfo.mExtent.mX) * 0.5f, static_cast<float>(mInfo.mExtent.mY) * 0.5f };
	}

	bool Viewport::isOpen() const
	{
		return !glfwWindowShouldClose(mWindow);
	}

	void Viewport::cursor(Cursor& cursor, Cursor::EMode mode)
	{
		glfwSetCursor(mWindow, cursor.cursor());
		cursorMode(mode);
	}

	void Viewport::defaultCursor()
	{
		glfwSetCursor(mWindow, nullptr);
	}

	void Viewport::cursorMode(Cursor::EMode mode)
	{
		if (mode == mCursorMode)
			return;

		mCursorMode = mode;
		glfwSetInputMode(mWindow,	GLFW_CURSOR, mCursorMode == Cursor::EMode::FREE ? GLFW_CURSOR_NORMAL :
									(mCursorMode == Cursor::EMode::HIDDEN ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_DISABLED));
	}

	void Viewport::cursorPosition(const math::Vec2f& kPosition)
	{
		glfwSetCursorPos(mWindow, kPosition.mX, kPosition.mY);
	}

	void Viewport::icon(Icon&& icon)
	{
		glfwSetWindowIcon(mWindow, icon.imageCount(), icon.images());
	}

	void Viewport::defaultIcon()
	{
		glfwSetWindowIcon(mWindow, 0, nullptr);
	}

	void Viewport::focus(uint8_t id)
	{
		mOnFocus(static_cast<uint8_t&&>(id));
	}

	void Viewport::loseFocus(uint8_t id)
	{
		mOnLostFocus(static_cast<uint8_t&&>(id));
	}

	void Viewport::close(uint8_t id)
	{
		mOnLostFocus(static_cast<uint8_t&&>(id));
		mOnClosed(static_cast<uint8_t&&>(id));
	}

	void Viewport::resize(uint8_t id, uint32_t width, uint32_t height)
	{
		if (mInfo.mExtent.mX == width && mInfo.mExtent.mY == height)
			return;

		mInfo.mExtent.mX = width;
		mInfo.mExtent.mY = height;

		mOnResized(static_cast<uint8_t&&>(id), mInfo.mExtent);
	}

	void Viewport::title(const char* kName)
	{
		if (mInfo.mTitle == kName)
			return;

		mInfo.mTitle = kName;
		glfwSetWindowTitle(mWindow, mInfo.mTitle.c_str());
	}

	GLFWwindow* Viewport::window()
	{
		return mWindow;
	}

	HWND Viewport::handle() const
	{
		return glfwGetWin32Window(mWindow);
	}

	HINSTANCE Viewport::handleInstance(HWND handle) const
	{
		return reinterpret_cast<HINSTANCE>(GetWindowLongPtr(handle, GWLP_HINSTANCE));
	}

	Viewport& Viewport::operator=(Viewport&& other)
	{
		if (mWindow)
			glfwDestroyWindow(mWindow);

		mOnResized = std::move(other.mOnResized);
		mOnFocus = std::move(other.mOnFocus);
		mOnLostFocus = std::move(other.mOnLostFocus);
		mOnClosed = std::move(other.mOnClosed);

		mInfo = std::move(other.mInfo);

		mWindow = other.mWindow;
		other.mWindow = nullptr;
		mCursorMode = other.mCursorMode;

		return *this;
	}
}