// 
// WindowEvent.cpp
// Core
// 
// Created by Felix Busch on 16.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#include <Core/Window/WindowEvent.hpp>

namespace Core
{
	////////////////////////////////////////////////////////////
	WindowEvent::WindowEvent()
	{
	}

	////////////////////////////////////////////////////////////
	WindowEvent::WindowEvent(EventType type):
		Type(type)
	{
	}
}