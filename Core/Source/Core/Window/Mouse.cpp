#include <Core/Window/Mouse.hpp>

#include <Windows.h>

namespace Core
{

	bool Mouse::isButtonPressed(Button button)
	{
		return GetAsyncKeyState((int)button) & 0x8000;
	}

	void Mouse::setPosition(i32_t x, i32_t y)
	{
		SetCursorPos(x, y);
	}

	void Mouse::setPosition(i32_t x, i32_t y, Windowhandle window)
	{
		POINT pos = { (LONG)x, (LONG)y };
		ClientToScreen(window, &pos);
		SetCursorPos(pos.x, pos.y);
	}

	void Mouse::setPosition(const IVector2 & position)
	{
		setPosition(position.x, position.y);
	}

	void Mouse::setPosition(const IVector2 & position, Windowhandle window)
	{
		setPosition(position.x, position.y, window);
	}

	IVector2 Mouse::getPosition()
	{
		POINT pos = { 0l, 0l };
		GetCursorPos(&pos);
		return IVector2(pos.x, pos.y);
	}

	IVector2 Mouse::getPosition(Windowhandle window)
	{
		POINT pos = { 0l, 0l };
		GetCursorPos(&pos);
		ScreenToClient(window, &pos);
		return IVector2(pos.x, pos.y);
	}

}