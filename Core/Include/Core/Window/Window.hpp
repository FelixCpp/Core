#pragma once

#include <string>

#include <Core/Maths/Vector2.hpp>

#include <Core/System/Datatypes.hpp>
#include <Core/System/TimeSpan.hpp>
#include <Core/System/Stopwatch.hpp>

#include <Core/Window/WindowEvents.hpp>
#include <Core/Window/Mouse.hpp>

#include <Core/Window/SystemCursor.hpp>
#include <Core/Window/SystemIcon.hpp>

#include <Core/Window/DisplayMode.hpp>

namespace Core
{

	class Window {
	public:

		/* the monitors width */
		static const i32_t displayWidth;
		
		/* the monitors height */
		static const i32_t displayHeight;

	public:

		/* the default constructor */
		Window();

		/* a virtual destructor because this class should be a baseclass. */
		virtual ~Window() = default;

		/* enters the fullscreen-mode with the specified properties of the displayMode parameter */
		bool EnterFullscreen(const DisplayMode & displayMode = DisplayMode::GetDesktopMode());

		/* exits the fullscreen-mode */
		bool ExitFullscreen(u32_t width, u32_t height);

		/* changes the framerate limit */
		void SetFramerateLimit(i32_t limit);

		/* releases the framerate limit */
		void NoFramerateLimit();

		/* creates a window with the given properties */
		virtual bool Create(u32_t width, u32_t height, const std::string & title) = 0;

		/* returns true if the window is currently open */
		bool IsOpen() const;

		/* closes the window */
		void Close();

		/* returns the state of the fullscreen attribute. For more description read the documentation of that field */
		bool IsFullscreen() const;

		/* changes the title of the window */
		void SetTitle(const std::string & title);

		/* returns the current title of the window */
		const std::string & GetTitle() const;

		/* changes the size of the window */
		void SetSize(u32_t width, u32_t height);
		
		/* returns the size of the window */
		UVector2 GetSize() const;

		/* changes the position of the window */
		void SetPosition(i32_t x, i32_t y);

		/* returns the position of the window */
		IVector2 GetPosition() const;

		/* changes the resizability of the window-edge */
		void SetResizable(bool resizable);

		/* returns true if the window is resizable on the edges */
		bool IsResizable() const;

		/* enables / disables the maximize-button in the top-right corner of the window */
		void SetMaximizable(bool maximizable);

		/* checks the state of the maximize-button and returns true if he is clickable */
		bool IsMaximizable() const;

		/* enables / disables the minimize-button in the top-right corner of the window */
		void SetMinimizable(bool minimizable);

		/* checks the state of the minimize-button and returns true if he is clickable */
		bool IsMinimizable() const;

		/* enables / disables the close-button in the top-right corner of the window */
		void SetClosable(bool closable);

		/* checks the state of the close-button and returns true if he is clickable */
		bool IsClosable() const;

		/* changes the visibility of the mouse cursor */
		void SetMouseCursorVisible(bool visible);

		/* returns true if the mouse cursor is visible */
		bool IsMouseCursorVisible() const;

		/* changes the state of the mouse cursor being grabbed */
		void SetMouseCursorGrabbed(bool grabbed);

		/* returns the state of the mouse cursor being grabbed */
		bool IsMouseCursorGrabbed() const;

		/* centers the window on the monitor */
		void Recenter();

		/* loads the file and changes the icon to match the read data */
		bool SetIcon(const std::string & filepath);

		/* loads the system icon and changes the icon to match it */
		bool SetIcon(SystemIcon icon);
		
		/* loads the file and changes the cursor to match the read data */
		bool SetCursor(const std::string & filepath);
		
		/* loads the system cursor and changes the cursor to match it */
		bool SetCursor(SystemCursor cursor);

		/* changes the visibility of the window */
		void SetVisible(bool visible);

		/* returns true if the window is visible */
		bool IsVisible() const;

	public:

		/* gets raised if any key is pressed */
		virtual void OnKeyPressed(KeyboardEventArgs args) {}

		/* gets raised if any key is released */
		virtual void OnKeyReleased(KeyboardEventArgs args) {}
		
		/* gets raised if a character was pressed */
		virtual void OnTextEntered(u32_t unicode) {}
		
		/* gets raised if any mouse button is pressed */
		virtual void OnMousePressed(Mouse::Button button) {}
		
		/* gets raised if any mouse button is released */
		virtual void OnMouseReleased(Mouse::Button button) {}
		
		/* gets raised if the mousewheel is scrolled */
		virtual void OnMouseWheelScrolled(i32_t delta) {}

		/* gets raised if the X-button is pressed */
		virtual void OnWindowClosed() {}

		/* gets raised if the window is resized */
		virtual void OnWindowResized() {}

		/* gets raised if the window was moved */
		virtual void OnWindowMoved() {}

		/* gets raised if the mouse was moved */
		virtual void OnMouseMoved() {}

		/* gets raised if the window gained focus */
		virtual void OnFocusGained() {}

		/* gets raised if the window lost focus */
		virtual void OnFocusLost() {}

		/* gets raised if the mouse left the boundary of the window */
		virtual void OnMouseLeft() {}

		/* gets raised if the mouse entered the boundary of the window */
		virtual void OnMouseEntered() {}

	protected:

		/* dispatches the windows events and raises the processEvents function */
		void DispatchEvents() const;

		/* calls the calculateFps and limitFps function */
		void HandleFps();

		/* grabs the mouse cursor based on the parameter */
		void GrabCursor(bool grabbed);

	private:

		/* calculates the fps */
		void CalculateFps();

		/* sleeps the right amount of time to match the fpslimit */
		void LimitFps();

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

		/* indicates wether the mouse cursor is grabbed inside the windows boundary or not */
		bool mouseCursorGrabbed;

		/// <summary>
		/// Indicates wether any mouse button
		/// is currently held down
		/// </summary>
		bool mouseIsPressed;

		/// <summary>
		/// Indicates wether any key
		/// is currently held down
		/// </summary>
		bool keyIsPressed;

		/// <summary>
		/// Indicates wether the key can be held down
		/// and the windows fires OnTextEntered events
		/// or just a single time if the key was pressed
		/// down
		/// </summary>
		bool keyRepeatEnabled;

	private:

		/* indicates the state of the cursors visibility */
		bool mouseCursorVisible;

		/* indicates wether the window is in fullscreen-mode or not */
		bool fullscreen;

	private: /* framerate limit */

		/* the fps-limit in duration format */
		TimeSpan fpsLimit;

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