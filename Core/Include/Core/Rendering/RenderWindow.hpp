#pragma once

#include <Core/Window/Window.hpp>

#include <Core/Rendering/RenderTarget.hpp>

namespace Core
{
	
	class GraphicsContext;
	class RenderStateManager;

	class RenderWindow : public Window, public RenderTarget {
	private:

		/* internal window-class identifier */
		inline static constexpr const char * LPSZ_CLASS_NAME = "Core_RenderWindow_Class";

	public:

		/* the default constructor */
		explicit RenderWindow(GraphicsContext *& gctx, RenderStateManager *& rsm);

		/* a virtual destructor */
		virtual ~RenderWindow();

		/* creates a window with the given properties */
		virtual bool create(u32_t width, u32_t height, const std::string & title) override;

	private:

		/* handles the events from windows */
		static i64_t __stdcall processEvents(Windowhandle handle, u32_t msg, u64_t wParam, i64_t lParam);

		/* decodes the key based on the flags. (Gets information which button is pressed) */
		static Keyboard::Key decodeKeyCode(u64_t key, i64_t flags);

		/* tracks the mouse events */
		void trackMouseEvent(bool track);

	private:

		/* indicates the state of the mouse wether its inside of the window boundary or not */
		bool mouseInsideWindow;

		/* indicates wether the user is currently resizing the window or not */
		bool resizing;

		/* a graphicscontext */
		GraphicsContext *& gctx;

	};

}