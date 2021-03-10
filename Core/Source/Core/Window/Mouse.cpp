#include <Core/Window/Mouse.hpp>

#include <Windows.h>

namespace Core
{

	bool Mouse::IsButtonPressed(Button button)
	{
		return GetAsyncKeyState((int)button) & 0x8000;
	}

	std::vector<Mouse::Button> Mouse::GetButtonsPressed(const Button * begin, const Button * end)
	{
		std::vector<Button> pressedButtons;
		
		for (const Button * button = begin; button != end; button++)
		{
			if (Mouse::IsButtonPressed(*button))
			{
				pressedButtons.push_back(*button);
			}
		}

		return pressedButtons;
	}

	void Mouse::SetPosition(Int32 x, Int32 y)
	{
		SetCursorPos(x, y);
	}

	void Mouse::SetPosition(Int32 x, Int32 y, Windowhandle window)
	{
		POINT pos = { (LONG)x, (LONG)y };
		ClientToScreen(window, &pos);
		SetCursorPos(pos.x, pos.y);
	}

	void Mouse::SetPosition(const IVector2 & position)
	{
		SetPosition(position.x, position.y);
	}

	void Mouse::SetPosition(const IVector2 & position, Windowhandle window)
	{
		SetPosition(position.x, position.y, window);
	}

	IVector2 Mouse::GetPosition()
	{
		POINT pos = { 0l, 0l };
		GetCursorPos(&pos);
		return IVector2(pos.x, pos.y);
	}

	IVector2 Mouse::GetPosition(Windowhandle window)
	{
		POINT pos = { 0l, 0l };
		GetCursorPos(&pos);
		ScreenToClient(window, &pos);
		return IVector2(pos.x, pos.y);
	}

}