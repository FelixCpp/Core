#pragma once

#include <Core/System/Datatypes.hpp>
#include <Core/System/Stopwatch.hpp>

#include <Core/Window/WindowEvents.hpp>
#include <Core/Window/SystemIcon.hpp>
#include <Core/Window/MouseCursor.hpp>

#include <Core/Application/RenderTarget.hpp>
#include <Core/Application/Image.hpp>

#include <string>

namespace Core
{

	class GraphicsContext;

	class Application : public RenderTarget {
	public:

		virtual ~Application();

		virtual void setup() {}
		virtual void draw() {}

		// pause & continue draw() call
		void pauseDrawing();
		void startDrawing();

		// handle framerate
		void frameRate(i32_t fpsLimit);

		// window related functions
		void setSize(u32_t width, u32_t height);
		void setPosition(i32_t x, i32_t y);
		void setTitle(const std::string & title);
		void recenter();
		bool setIcon(const std::string & filepath); // if the filepath is empty, the window releases the current icon
		bool setIcon(SystemIcon icon);
		bool setCursor(const std::string & filepath);
		bool setCursor(MouseCursor cursor);
		void setMouseCursorVisible(bool visible);
		void exit();

		template<class T, typename ... TArgs>
		static void launch(const TArgs & ... arguments)
		{
			T instance(std::forward<decltype(arguments)>(arguments)...);
			instance.startSketch();
		}

	protected:

		explicit Application(i32_t width = 200, i32_t height = 200, const std::string & title = "Core - Application");

	protected:

		virtual void onKeyPressed(KeyboardEventArgs args) {}
		virtual void onKeyReleased(KeyboardEventArgs args) {}
		virtual void onTextEntered(Keyboard::Key key) {}
		virtual void onMousePressed(Mouse::Button button) {}
		virtual void onMouseReleased(Mouse::Button button) {}
		virtual void onMouseWheelScrolled(i32_t delta) {}
		
		virtual void onWindowClosed() {}
		virtual void onWindowResized() {}
		virtual void onWindowMoved() {}
		virtual void onMouseMoved() {}

		virtual void onFocusGained() {}
		virtual void onFocusLost() {}
		
		virtual void onMouseLeft() {}
		virtual void onMouseEntered() {}

	private:

		void setupImpl();
		void drawImpl();

		void calculateFrameRate();
		void limitFrameRate();

		void createWindow(i32_t width, i32_t height, const std::string & title);
		void startSketch();
		void dispatchEvents();
		void trackMouseEvent(bool active);

		static Keyboard::Key decodeKeyCode(u64_t wParam, i64_t lParam);
		static i64_t __stdcall handleEvents(Windowhandle handle, u32_t msg, u64_t wParam, i64_t lParam);

	public:

		i32_t width;
		i32_t height;

		i32_t windowX;
		i32_t windowY;

		i32_t mouseX;
		i32_t mouseY;

		i32_t pmouseX;
		i32_t pmouseY;

		i32_t frameCount;
		i32_t fps;

		std::string title;

		Windowhandle windowHandle;

		Resourcehandle cursorHandle;
		Resourcehandle iconHandle;

	private:

		bool isOpen;
		bool drawingPaused;
		bool mouseInsideWindow;
		bool mouseCursorVisible;

		const char * lpszClassName;

		GraphicsContext * gctx;
		
		Duration fpsLimit;
		Stopwatch delayWatch;
		Stopwatch fpsWatch;
		Stopwatch calcWatch;

	};

}