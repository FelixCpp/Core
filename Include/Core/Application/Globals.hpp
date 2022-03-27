// 
// Globals.hpp
// Core
// 
// Created by Felix Busch on 27.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/System/Types.hpp>
#include <Core/System/IEventListener.hpp>
#include <Core/Window/WindowEvent.hpp>

namespace Core
{

	extern const i32& Width;
	extern const i32& Height;
	extern const i32& MouseX;
	extern const i32& MouseY;
	extern const i32& PMouseX;
	extern const i32& PMouseY;

	////////////////////////////////////////////////////////////
	/// \brief Define event listener that updates the global
	///		   variables.
	/// 
	////////////////////////////////////////////////////////////
	class GlobalUpdatingService final : public IEventListener<WindowEvent>
	{
	public:

		////////////////////////////////////////////////////////////
		/// \brief Callback method for window events.
		/// 
		////////////////////////////////////////////////////////////
		virtual void OnEvent(const WindowEvent& event) override;

	};

}