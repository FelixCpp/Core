// 
// WindowEvent.hpp
// Core
// 
// Created by Felix Busch on 16.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/Window/MouseWheel.hpp>
#include <Core/Window/MouseButton.hpp>
#include <Core/Window/KeyCode.hpp>
#include <Core/Window/Types.hpp>

#include <cstdint>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define window event
	/// 
	////////////////////////////////////////////////////////////
	struct WindowEvent
	{
		////////////////////////////////////////////////////////////
		/// \brief This structure is filled with raw event data
		/// 
		////////////////////////////////////////////////////////////
		struct AnyEvent
		{
			WindowHandle Handle;
			MessageID Message;
			WParam WParam;
			LParam LParam;
		};

		////////////////////////////////////////////////////////////
		/// \brief Resize event
		/// 
		////////////////////////////////////////////////////////////
		struct SizeEvent
		{
			uint32_t Width;
			uint32_t Height;
		};
		
		////////////////////////////////////////////////////////////
		/// \brief The mouse wheel has been scrolled
		/// 
		////////////////////////////////////////////////////////////
		struct MouseWheelScrollEvent
		{
			MouseWheel	Wheel;	//!< The wheel that has been scrolled
			float		Delta;	//!< The delta that indicates the direction
			int32_t		MouseX; //!< The horizontal mouse coordinate
			int32_t		MouseY;	//!< The vertical mouse coordinate
		};

		////////////////////////////////////////////////////////////
		/// \brief The mouse has been moved
		/// 
		////////////////////////////////////////////////////////////
		struct MouseMoveEvent
		{
			int32_t MouseX;	//!< The horizontal mouse coordinate
			int32_t MouseY;	//!< The vertical mouse coordinate
		};

		////////////////////////////////////////////////////////////
		/// \brief The mouse button has been pressed or released
		/// 
		////////////////////////////////////////////////////////////
		struct MouseButtonEvent
		{
			MouseButton Button;	//!< The button that has been pressed or released
			int32_t		MouseX;	//!< The horizontal mouse coordinate
			int32_t		MouseY;	//!< The vertical mouse coordinate
		};

		////////////////////////////////////////////////////////////
		/// \brief A key has been pressed or released
		/// 
		////////////////////////////////////////////////////////////
		struct KeyEvent
		{
			KeyCode	Code;		//!< The key code that has been pressed/released
			bool	Alt;		//!< Whether the Alt key was down during the key press
			bool	Control;	//!< Whether the Control key was down during the key press
			bool	Shift;		//!< Whether the Shift key was down during the key press
			bool	System;		//!< Whether the System key (e.g. windows-key) was down during the key press
		};

		////////////////////////////////////////////////////////////
		/// \brief The user typed text
		/// 
		////////////////////////////////////////////////////////////
		struct TextEvent
		{
			uint32_t Unicode; //!< Unicode representation of the typed character
		};

		////////////////////////////////////////////////////////////
		/// \brief The type to decide which data to use
		/// 
		////////////////////////////////////////////////////////////
		enum EventType
		{
			Closed,				//!< A close of the window is requested (no data)
			Resized,			//!< The window has been resized (data in WindowEvent::Size)
			MouseWheelScrolled,	//!< The mouse wheel has been scrolled (data in WindowEvent::MouseWheel)
			MouseMoved,			//!< The mouse position has been moved (data in WindowEvent::MouseMove)
			MouseLeft,			//!< The mouse has left the window boundary (no data)
			MouseEntered,		//!< The mouse has entered the window boundary (no data)
			MousePressed,		//!< Any mouse button has been pressed (data in WindowEvent::MouseButton)
			MouseReleased,		//!< Any mouse button has been released (data in WindowEvent::MouseButton)
			KeyPressed,			//!< Any key has been pressed (data in WindowEvent::Key)
			KeyReleased,		//!< Any key has been released (data in WindowEvent::Key)
			TextEntered,		//!< The user typed some text (data in WindowEvent::Text)
			FocusGained,		//!< The window gained focus (no data)
			FocusLost,			//!< The window lost focus (no data)
			Generic				//!< Any event (data in WindowEvent::Any)
		};

		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		/// 
		////////////////////////////////////////////////////////////
		WindowEvent();

		////////////////////////////////////////////////////////////
		/// \brief Construct WindowEvent with its type directly
		/// 
		////////////////////////////////////////////////////////////
		WindowEvent(EventType type);

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		EventType Type;

		////////////////////////////////////////////////////////////
		/// \brief Event data
		/// 
		////////////////////////////////////////////////////////////
		union
		{
			SizeEvent				Size;				//!< Resize information (data of EventType::Resized)
			MouseWheelScrollEvent	MouseWheel;			//!< Mouse wheel scroll information (data of EventType::MouseWheelScrolled)
			MouseMoveEvent			MouseMove;			//!< Mouse move information (data of EventType::MouseMoved)
			MouseButtonEvent		MouseButton;		//!< Mouse button has been pressed / released (data of EventType::MousePressed, EventType::MouseReleased)
			KeyEvent				Key;				//!< Key has been pressed or released (data of EventType::KeyPressed, EventType::KeyReleased)
			TextEvent				Text;				//!< Text is typed (data of EventType::TextEntered)
			AnyEvent				Any;				//!< Any event occurred (data of EventType::Generic)
		};

	};

}