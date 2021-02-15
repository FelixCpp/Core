#include <Core/Window/Keyboard.hpp>

#include <Windows.h>

namespace Core
{

	bool Keyboard::IsKeyPressed(Key key)
	{
		return GetAsyncKeyState((int)key) & 0x8000;
	}

	std::vector<Keyboard::Key> Keyboard::GetKeysPressed(const Key * begin, const Key * end)
	{
		std::vector<Key> keysPressed;

		for (const Key * key = begin; key != end; key++)
		{
			if (Keyboard::IsKeyPressed(*key))
			{
				keysPressed.push_back(*key);
			}
		}

		return keysPressed;
	}

}