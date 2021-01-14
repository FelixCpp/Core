#include <Core/Window/Keyboard.hpp>

#include <Windows.h>

namespace Core
{

	bool Keyboard::IsKeyPressed(Key key)
	{
		return GetAsyncKeyState((int)key) & 0x8000;
	}

	std::vector<Keyboard::Key> Keyboard::AreKeysPressed(const std::vector<Key> & keys)
	{
		std::vector<Key> keysPressed;

		for (const Key & key : keys)
		{
			if (Keyboard::IsKeyPressed(key))
			{
				keysPressed.push_back(key);
			}
		}

		return keysPressed;
	}

}