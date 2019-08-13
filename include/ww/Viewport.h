#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <NonCopyable.h>
#include <ISerializable.h>
#include <Event.h>

#include "Cursor.h"
#include "Icon.h"

namespace maj::ww
{
	class Viewport final : public NonCopyable
	{
	public:
		struct Info final : public ISerializable
		{
			ENABLE_META(Info);

		public:
			LOAD_CONSTRUCTOR(Info)

			Info(const math::Vec2ui& kExtent, const char* kTitle);
			Info(const Info& kOther);

			Info& operator=(const Info& kOther);

			math::Vec2ui mExtent;
			std::string mTitle;
		};

		Viewport(const Info& kInfo);
		Viewport(Viewport&& other);
		~Viewport();

		const math::Vec2ui& extent() const;
		math::Vec2f center() const;

		bool isOpen() const;
		
		void cursor(Cursor& cursor, Cursor::EMode mode);
		void defaultCursor();
		void cursorMode(Cursor::EMode mode);
		void cursorPosition(const math::Vec2f& kPosition);

		void icon(Icon&& icon);
		void defaultIcon();

		void focus(uint8_t id);
		void loseFocus(uint8_t id);
		void close(uint8_t id);
		void resize(uint8_t id, uint32_t width, uint32_t height);
		void title(const char* kName);

		GLFWwindow* window();
		HWND handle() const;
		HINSTANCE handleInstance(HWND handle) const;

		Viewport& operator=(Viewport&& other);

		Event<void, uint8_t, const math::Vec2ui&> mOnResized;
		Event<void, uint8_t> mOnFocus;
		Event<void, uint8_t> mOnLostFocus;
		Event<void, uint8_t> mOnClosed;

	private:
		Info mInfo;
		GLFWwindow* mWindow;
		Cursor::EMode mCursorMode;
	};
}

META_CLASS(maj::ww::Viewport::Info, 2u)
META_FIELD(maj::ww::Viewport::Info, math::Vec2ui, mExtent, 0u)
META_FIELD(maj::ww::Viewport::Info, std::string, mTitle, 1u)

#endif