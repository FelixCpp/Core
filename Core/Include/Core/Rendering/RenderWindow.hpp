#pragma once

/// <summary>
/// Core
/// </summary>

/// <summary>
/// Window components
/// </summary>
#include <Core/Window/Window.hpp>

/// <summary>
/// Rendering components
/// </summary>
#include <Core/Rendering/Targets/RenderTarget.hpp>
#include <Core/Rendering/FrameSaver.hpp>

/// <summary>
/// System components
/// </summary>
#include <Core/System/LateRef.hpp>

namespace Core
{
	
	class Renderer;
	class RenderStateManager;

	class RenderWindow : public Window, public RenderTarget, public FrameSaver {
	private:

		/* internal window-class identifier */
		inline static constexpr const char * LPSZ_CLASS_NAME = "Core_RenderWindow_Class";

	public:

		/* the default constructor */
		explicit RenderWindow(LateRef<Renderer> renderer, LateRef<RenderStateManager> rsm);

		/* a virtual destructor */
		virtual ~RenderWindow();

		/* creates a window with the given properties */
		virtual bool Create(u32_t width, u32_t height, const std::string & title) override;

	private:

		/* handles the events from windows */
		static i64_t __stdcall ProcessEvents(Windowhandle handle, u32_t msg, u64_t wParam, i64_t lParam);

		/* decodes the key based on the flags. (Gets information which button is pressed) */
		static Keyboard::Key DecodeKeyCode(u64_t key, i64_t flags);

		/* tracks the mouse events */
		void TrackMouseEvent(bool track);

	private:

		/* indicates the state of the mouse wether its inside of the window boundary or not */
		bool mouseInsideWindow;

		/* indicates wether the user is currently resizing the window or not */
		bool resizing;

		/* a Renderer */
		LateRef<Renderer> renderer;

	};

}