#pragma once

#include <Core/Window/Keyboard.hpp>

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