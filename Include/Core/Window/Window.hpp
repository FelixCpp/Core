// 
// Window.hpp
// Core
// 
// Created by Felix Busch on 15.02.2022.
// Copyright � 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/System/String.hpp>
#include <Core/System/EventPublisher.hpp>
#include <Core/System/Value2.hpp>

#include <Core/Window/WindowEvent.hpp>
#include <Core/Window/MouseCursor.hpp>
#include <Core/Window/Types.hpp>

#include <cstdint>
#include <memory>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define window class
	/// 
	////////////////////////////////////////////////////////////
	class Window final : public EventPublisher<WindowEvent>
	{
	private:

		////////////////////////////////////////////////////////////
		/// Static variables
		/// 
		////////////////////////////////////////////////////////////
		static uint32_t WindowCount; //!< The number of windows running at the same time

	public:

		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		/// 
		////////////////////////////////////////////////////////////
		Window();
		
		////////////////////////////////////////////////////////////
		/// \brief Destructor
		/// 
		////////////////////////////////////////////////////////////
		virtual ~Window() override;

		////////////////////////////////////////////////////////////
		/// Delete copy & move constructors/assignments
		/// 
		////////////////////////////////////////////////////////////
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator = (const Window&) = delete;
		Window& operator = (Window&&) = delete;

		////////////////////////////////////////////////////////////
		/// \brief Opens a new window in the middle of the monitor
		///
		/// If the window is opened and Open() is called,
		///	the window will be closed and re-opened with the
		///	given size.
		///
		///	\param width The width of the window
		///	\param height The height of the window
		///	\param title The title of the window
		///
		/// \return True if the window is opened successfully, false otherwise
		///
		////////////////////////////////////////////////////////////
		[[nodiscard]] bool Open(int32_t width, int32_t height, const String& title);

		////////////////////////////////////////////////////////////
		/// \brief Posts close message to windows callback
		///
		///	This method does not actually close the window.
		/// Instead it simply creates a Close event and queues
		///	it up in the event queue. The user will be notified next
		///	time dispatching these events and handle it by himself.
		/// 
		////////////////////////////////////////////////////////////
		void Close();

		////////////////////////////////////////////////////////////
		/// \brief This method actually destroys the window without
		///		   sending a Close event.
		/// 
		////////////////////////////////////////////////////////////
		void Destroy();

		////////////////////////////////////////////////////////////
		/// \brief Get whether the window is opened or closed
		///
		///	\return True if the window is open, false otherwise
		/// 
		////////////////////////////////////////////////////////////
		[[nodiscard]] bool IsOpen() const;

		////////////////////////////////////////////////////////////
		/// \brief Detect whether the window is in fullscreen mode
		///		   or not
		///
		/// \return True if the window is currently in fullscreen mode,
		///			false otherwise
		///
		////////////////////////////////////////////////////////////
		[[nodiscard]] bool IsFullscreenWindow() const;

		////////////////////////////////////////////////////////////
		/// \brief Set the size of the window
		///
		///	\param width The new width of the window
		///	\param height The new height of the window
		/// 
		////////////////////////////////////////////////////////////
		void SetSize(uint32_t width, uint32_t height);

		////////////////////////////////////////////////////////////
		/// \brief Set the inner size of the window
		///
		///	\param size The new size of the window
		/// 
		////////////////////////////////////////////////////////////
		void SetSize(const UInt2& size);

		////////////////////////////////////////////////////////////
		/// \brief Get the current size of the window
		///
		///	\return The size of the window
		/// 
		////////////////////////////////////////////////////////////
		[[nodiscard]] UInt2 GetSize() const;

		////////////////////////////////////////////////////////////
		/// \brief Set the position of the window
		///
		///	\param x The horizontal coordinate
		///	\param y The vertical coordinate
		/// 
		////////////////////////////////////////////////////////////
		void SetPosition(int32_t x, int32_t y);

		////////////////////////////////////////////////////////////
		/// \brief Get the position of the window
		/// 
		////////////////////////////////////////////////////////////
		[[nodiscard]] Int2 GetPosition() const;

		////////////////////////////////////////////////////////////
		/// \brief Set the window title to match the given parameter
		///
		///	\param title The new title of the window
		/// 
		////////////////////////////////////////////////////////////
		void SetTitle(const String& title);

		////////////////////////////////////////////////////////////
		/// \brief Get the title of the window
		///
		///	\return The current title of the window
		/// 
		////////////////////////////////////////////////////////////
		[[nodiscard]] String GetTitle() const;

		////////////////////////////////////////////////////////////
		/// \brief Enable/disable key repetitions
		///
		///	Enabled:
		///		The window posts events over and over again while the
		///		key is held down
		///
		///	Disables:
		///		The window posts only one event when a key is pressed
		/// 
		////////////////////////////////////////////////////////////
		void SetKeyRepeatEnabled(bool enabled);

		////////////////////////////////////////////////////////////
		/// \brief Get state of key repetitions
		///
		///	Enabled:
		///		The window posts events over and over again while the
		///		key is held down
		///
		///	Disables:
		///		The window posts only one event when a key is pressed
		///
		///	\return True if key repetitions is enabled
		///
		////////////////////////////////////////////////////////////
		[[nodiscard]] bool IsKeyRepeatEnabled() const;

		////////////////////////////////////////////////////////////
		/// \brief Enable/Disable window sizing buttons
		/// 
		////////////////////////////////////////////////////////////
		void SetMaximizeButtonEnabled(bool enabled);
		[[nodiscard]] bool IsMaximizeButtonEnabled() const;

		void SetMinimizeButtonEnabled(bool enabled);
		[[nodiscard]] bool IsMinimizeButtonEnabled() const;

		void SetCloseButtonEnabled(bool enabled);
		[[nodiscard]] bool IsCloseButtonEnabled() const;

		////////////////////////////////////////////////////////////
		/// \brief Enable/Disable 
		/// 
		////////////////////////////////////////////////////////////
		void SetResizable(bool enabled);
		[[nodiscard]] bool IsResizable() const;

		////////////////////////////////////////////////////////////
		/// \brief Shows/Hides the window
		///
		///	\param visible The new visibility of the window
		/// 
		////////////////////////////////////////////////////////////
		void SetVisible(bool visible);

		////////////////////////////////////////////////////////////
		/// \brief Get the visibility of the window
		///
		///	\return True if the window is visible
		/// 
		////////////////////////////////////////////////////////////
		[[nodiscard]] bool IsVisible() const;

		////////////////////////////////////////////////////////////
		/// \brief Change visibility of the mouse cursor
		/// 
		////////////////////////////////////////////////////////////
		void SetMouseCursorVisible(bool visible);

		////////////////////////////////////////////////////////////
		/// \brief Get the visibility of the mouse cursor
		/// 
		////////////////////////////////////////////////////////////
		[[nodiscard]] bool IsMouseCursorVisible() const;

		////////////////////////////////////////////////////////////
		/// \brief Grabs/Frees the mouse cursor
		/// 
		////////////////////////////////////////////////////////////
		void SetMouseCursorGrabbed(bool grabbed);

		////////////////////////////////////////////////////////////
		/// \brief Get the grabbing state of the mouse cursor
		/// 
		////////////////////////////////////////////////////////////
		[[nodiscard]] bool IsMouseCursorGrabbed();

		////////////////////////////////////////////////////////////
		/// \brief Focus
		/// 
		////////////////////////////////////////////////////////////
		void RequestFocus();
		[[nodiscard]] bool HasFocus() const;

		////////////////////////////////////////////////////////////
		/// \brief Set the mouse cursor for the window handle
		/// 
		////////////////////////////////////////////////////////////
		void SetMouseCursor(const MouseCursor& cursor);
		const MouseCursor& GetMouseCursor() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the window handle
		/// 
		////////////////////////////////////////////////////////////
		[[nodiscard]] WindowHandle GetWindowHandle() const;

	private:

		////////////////////////////////////////////////////////////
		/// \brief Create the window class that interacts with the OS
		/// 
		////////////////////////////////////////////////////////////
		void InitWindowClass() const;

		////////////////////////////////////////////////////////////
		/// \brief Enable/Disable window styles
		/// 
		////////////////////////////////////////////////////////////
		void SetWindowStyle(int64_t style, bool enabled);

		////////////////////////////////////////////////////////////
		/// \brief Get whether the window style is enabled
		/// 
		////////////////////////////////////////////////////////////
		[[nodiscard]] bool IsWindowStyleEnabled(int64_t style) const;
		
	protected:

		////////////////////////////////////////////////////////////
		/// \brief Overwritten method to queue up events using the
		///		   win32 API
		/// 
		////////////////////////////////////////////////////////////
		virtual void QueueEvents() override;

	private:

		////////////////////////////////////////////////////////////
		/// \brief Implementation class
		/// 
		////////////////////////////////////////////////////////////
		class Impl;

		////////////////////////////////////////////////////////////
		/// Private member data
		/// 
		////////////////////////////////////////////////////////////
		std::unique_ptr<Impl>	impl;	///< The implementation of this class
		MouseCursor				cursor;	///< The mouse cursor held by the window

	};

}
