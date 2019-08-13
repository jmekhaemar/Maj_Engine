#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <unordered_map>

#include <Service.h>

#include "Viewport.h"
#include "Input.h"

namespace maj::ww
{
	class Window final : public Service
	{
	private:
		struct Context final : public NonCopyable
		{
			Context();
			~Context();
		};

		static void onError(int32_t error, const char* kDescription);
		static void onViewportResized(GLFWwindow* window, int32_t width, int32_t height);
		static void onViewportFocused(GLFWwindow* window, int32_t focus);
		static void onViewportClosed(GLFWwindow* window);

	public:
		// Viewport index must be between [ 0 ; skMaxViewportCount - 1 ]
		// Must be between [ 1 ; 254 ]
		static const uint8_t skMaxViewportCount;

	public:
		Window(const Viewport::Info& kInfo);
		~Window() = default;

		bool isAnyOpen() const;
		void pollEvents();

		void focusViewport(uint8_t i);
		bool addViewport(Viewport::Info&& info);
		void removeViewport(uint8_t i);
		
		uint8_t viewportCount() const;
		const std::unordered_map<uint8_t, Viewport>& viewports() const;

		uint8_t focusViewport() const;
		const Viewport& viewport(uint8_t i) const;
		Viewport& viewport(uint8_t i);

		const char** extensionsRequired(uint32_t& count) const;

		Event<void, uint8_t, Viewport&> mOnViewportAdded;

	private:
		Context mContext;

		std::unordered_map<uint8_t, Viewport> mViewports;
		uint8_t mFocusViewport;

		Input mInput;
	};
}

#endif