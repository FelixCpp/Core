#pragma once

#include <string>

#include <Core/Maths/Vector2.hpp>

#include <Core/System/Datatypes.hpp>
#include <Core/System/Duration.hpp>
#include <Core/System/Stopwatch.hpp>

#include <Core/Window/WindowEvents.hpp>
#include <Core/Window/Mouse.hpp>

#include <Core/Window/SystemCursor.hpp>
#include <Core/Window/SystemIcon.hpp>

#include <Core/Window/DisplayMode.hpp>

namespace Core
{

	class Window {
	private:

		/* internal window-class identifier */
		inline static constexpr const char * LPSZ_CLASS_NAME = "Core_Window_Class";

	public:

		/* the monitors width */
		static const i32_t displayWidth;
		
		/* the monitors height */
		static const i32_t displayHeight;

	public:

		/* the default constructor */
		Window();

		/* a virtual destructor because this class should be a baseclass. */
		virtual ~Window();

		/* enters the fullscreen-mode with the specified properties of the displayMode parameter */
		bool enterFullscreen(const DisplayMode & displayMode);

		/* exits the fullscreen-mode */
		bool exitFullscreen(u32_t width, u32_t height);

		/* changes the framerate limit */
		void setFramerateLimit(i32_t limit);

		/* releases the framerate limit */
		void noFramerateLimit();

		/* creates a window with the given properties */
		bool create(u32_t width, u32_t height, const std::string & title);

		/* returns true if the window is currently open */
		bool isOpen() const;

		/* closes the window */
		void close();

		/* returns the state of the fullscreen attribute. For more description read the documentation of that field */
		bool isFullscreen() const;

		/* changes the title of the window */
		void setTitle(const std::string & title);

		/* returns the current title of the window */
		const std::string & getTitle() const;

		/* changes the size of the window */
		void setSize(u32_t width, u32_t height);
		
		/* returns the size of the window */
		UVector2 getSize() const;

		/* changes the position of the window */
		void setPosition(i32_t x, i32_t y);

		/* returns the position of the window */
		IVector2 getPosition() const;

		/* changes the resizability of the window-edge */
		void setResizable(bool resizable);

		/* returns true if the window is resizable on the edges */
		bool isResizable() const;

		/* enables / disables the maximize-button in the top-right corner of the window */
		void setMaximizable(bool maximizable);

		/* checks the state of the maximize-button and returns true if he is clickable */
		bool isMaximizable() const;

		/* enables / disables the minimize-button in the top-right corner of the window */
		void setMinimizable(bool minimizable);

		/* checks the state of the minimize-button and returns true if he is clickable */
		bool isMinimizable() const;

		/* enables / disables the close-button in the top-right corner of the window */
		void setClosable(bool closable);

		/* checks the state of the close-button and returns true if he is clickable */
		bool isClosable() const;

		/* changes the visibility of the mouse cursor */
		void setMouseCursorVisible(bool visible);

		/* returns true if the mouse cursor is visible */
		bool isMouseCursorVisible() const;

		/* changes the state of the mouse cursor being grabbed */
		void setMouseCursorGrabbed(bool grabbed);

		/* returns the state of the mouse cursor being grabbed */
		bool isMouseCursorGrabbed() const;

		/* centers the window on the monitor */
		void recenter();

		/* loads the file and changes the icon to match the read data */
		bool setIcon(const std::string & filepath);

		/* loads the system icon and changes the icon to match it */
		bool setIcon(SystemIcon icon);
		
		/* loads the file and changes the cursor to match the read data */
		bool setCursor(const std::string & filepath);
		
		/* loads the system cursor and changes the cursor to match it */
		bool setCursor(SystemCursor cursor);

		/* changes the visibility of the window */
		void setVisible(bool visible);

		/* returns true if the window is visible */
		bool isVisible() const;

	public:

		/* gets raised if any key is pressed */
		virtual void onKeyPressed(KeyboardEventArgs args) {}

		/* gets raised if any key is released */
		virtual void onKeyReleased(KeyboardEventArgs args) {}
		
		/* gets raised if a character was pressed */
		virtual void onTextEntered(Keyboard::Key key) {}
		
		/* gets raised if any mouse button is pressed */
		virtual void onMousePressed(Mouse::Button button) {}
		
		/* gets raised if any mouse button is released */
		virtual void onMouseReleased(Mouse::Button button) {}
		
		/* gets raised if the mousewheel is scrolled */
		virtual void onMouseWheelScrolled(i32_t delta) {}

		/* gets raised if the X-button is pressed */
		virtual void onWindowClosed() {}

		/* gets raised if the window is resized */
		virtual void onWindowResized() {}

		/* gets raised if the window was moved */
		virtual void onWindowMoved() {}

		/* gets raised if the mouse was moved */
		virtual void onMouseMoved() {}

		/* gets raised if the window gained focus */
		virtual void onFocusGained() {}

		/* gets raised if the window lost focus */
		virtual void onFocusLost() {}

		/* gets raised if the mouse left the boundary of the window */
		virtual void onMouseLeft() {}

		/* gets raised if the mouse entered the boundary of the window */
		virtual void onMouseEntered() {}

	protected:

		/* dispatches the windows events and raises the processEvents function */
		void dispatchEvents() const;

		/* calls the calculateFps and limitFps function */
		void handleFps();

		/* destroys the handles */
		void destroy();

		/* grabs the mouse cursor based on the parameter */
		void grabCursor(bool grabbed);

	private:

		/* handles the events from windows */
		static i64_t __stdcall processEvents(Windowhandle handle, u32_t msg, u64_t wParam, i64_t lParam);

		/* decodes the key based on the flags. (Gets information which button is pressed) */
		static Keyboard::Key decodeKeyCode(u64_t key, i64_t flags);

		/* tracks the mouse events */
		void trackMouseEvent(bool track);

		/* calculates the fps */
		void calculateFps();

		/* sleeps the right amount of time to match the fpslimit */
		void limitFps();

	public:

		/* the width of the window */
		i32_t width;

		/* the height of the window */
		i32_t height;

		/* the previous mouse-x position */
		i32_t pmouseX;

		/* the previous mouse-y position */
		i32_t pmouseY;

		/* the mouse-x position relative to the windows x position */
		i32_t mouseX;

		/* the mouse-y position relative to the windows y position */
		i32_t mouseY;

		/* the window-x position relative to the monitors x position (0) */
		i32_t windowX;

		/* the window-y position relative to the monitors y position (0) */
		i32_t windowY;

		/* the current title of the window */
		std::string title;

		/* the current framerate. It gets recalculated every 0.25 seconds */
		i32_t fps;

		/* the framecount. This value increases every frame and never gets resetted */
		u64_t frameCount;

		/* holds the handle of the window */
		Windowhandle windowHandle;

		/* holds the handle of the mouse-cursor */
		Resourcehandle cursorHandle;

		/* holds the handle for the window-icon */
		Resourcehandle iconHandle;

	private:

		/* indicates the state of the cursors visibility */
		bool mouseCursorVisible;

		/* indicates wether the mouse cursor is grabbed inside the windows boundary or not */
		bool mouseCursorGrabbed;

		/* indicates the state of the mouse wether its inside of the window boundary or not */
		bool mouseInsideWindow;
		
		/* indicates wether the window was closed after creation or not */
		bool open;

		/* indicates wether the window is in fullscreen-mode or not */
		bool fullscreen;

		/* indicates wether the user is currently resizing the window or not */
		bool resizing;

	private: /* framerate limit */

		/* the fps-limit in duration format */
		Duration fpsLimit;

		/* clock to delay the recalculation of the fps */
		Stopwatch delayWatch;
		
		/* clock which is needed to calculate the fps */
		Stopwatch fpsWatch;

		/* clock which is needed to sleep the right amount of time to match the fpslimit */
		Stopwatch calcWatch;

		/* frameCount which gets increased everytime when handleFps() is called */
		u32_t internalFrameCount;

	};

}