#pragma once

#include <Core/Window/Keyboard.hpp>

namespace Core
{
	
	struct KeyboardEventArgs {
		bool alt;			 /* if this flag is true it means, that the alt key was down while pressing the key (code) */
		bool control;		 /* if this flag is true it means, that the control key was down while pressing the key (code) */
		bool shift;			 /* if this flag is true it means, that the shift key was down while pressing the key (code) */
		bool system;		 /* if this flag is true it means, that the system key was down while pressing the key (code) */
		Keyboard::Key code;	 /* the actual key which was pressed */
	};

}