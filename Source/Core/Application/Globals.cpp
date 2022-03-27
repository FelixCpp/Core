// 
// Globals.cpp
// Core
// 
// Created by Felix Busch on 27.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#include <Core/Application/Globals.hpp>

namespace Core
{
	namespace Internal
	{
		static i32 width = 0;
		static i32 height = 0;
		static i32 mouseX = 0;
		static i32 mouseY = 0;
		static i32 pMouseX = 0;
		static i32 pMouseY = 0;
	}

	const i32& Width	= Internal::width;
	const i32& Height	= Internal::height;
	const i32& MouseX	= Internal::mouseX;
	const i32& MouseY	= Internal::mouseY;
	const i32& PMouseX	= Internal::pMouseX;
	const i32& PMouseY	= Internal::pMouseY;

	////////////////////////////////////////////////////////////
	void GlobalUpdatingService::OnEvent(const WindowEvent& event)
	{
		switch (event.Type)
		{
			using namespace Internal;

			// the window has been resized
			case WindowEvent::Resized:
			{
				width  = (i32)event.Size.Width;
				height = (i32)event.Size.Height;
			} break;

			// the mouse has been moved
			case WindowEvent::MouseMoved:
			{
				pMouseX = mouseX;
				pMouseY = mouseY;
				mouseX = event.MouseMove.MouseX;
				mouseY = event.MouseMove.MouseY;
			} break;

			default:
				break;
		}
	}


}