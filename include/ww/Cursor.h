#ifndef _CURSOR_H_
#define _CURSOR_H_

#include <GLFW/glfw3.h>

#include <NonCopyable.h>
#include <Vec2.h>

namespace maj::ww
{
	class Cursor final : public NonCopyable
	{
	public:
		enum class EShape
		{
			ARROW,
			I_BEAM,
			CROSSHAIR,
			HAND,
			RESIZE_X,
			RESIZE_Y
		};

		enum class EMode
		{
			FREE,
			HIDDEN,
			FORCE
		};

	private:
		static constexpr int32_t convertShape(EShape shape);

	public:
		Cursor(EShape shape);
		Cursor(const char* kPath, const math::Vec2i& kHotspot);
		~Cursor();

		GLFWcursor* cursor();

	private:
		GLFWcursor* mCursor;
	};
}

#endif