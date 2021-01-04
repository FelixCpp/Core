#include <Core/Window/Keyboard.hpp>

#include <Windows.h>

namespace Core
{

	bool Keyboard::isKeyPressed(Key key)
	{
		return GetAsyncKeyState((int)key) & 0x8000;
	}

}