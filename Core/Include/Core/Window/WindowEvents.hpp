#pragma once

#include <Core/System/Datatypes.hpp>
#include <Core/System/Event.hpp>

#include <Core/Maths/Vector2.hpp>

#include <Core/Window/Keyboard.hpp>
#include <Core/Window/Mouse.hpp>

namespace Core
{
	
	struct KeyboardEventArgs {
		bool alt;
		bool control;
		bool shift;
		bool system;
		Keyboard::Key code;
	};

}