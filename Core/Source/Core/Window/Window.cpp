#include <Core/Window/Window.hpp>
#include <Core/Rendering/GraphicsContext.hpp>

#include <Core/System/Logger.hpp>

#include <Windows.h>
#include <windowsx.h>

#include <thread>

namespace Core
{

	const i32_t Window::displayWidth = DisplayMode::getDesktopMode().width;
	const i32_t Window::displayHeight = DisplayMode::getDesktopMode().height;

	Window::Window(GraphicsContext *& gctx) :
		width(0),
		height(0),
		pmouseX(0),
		pmouseY(0),
		mouseX(0),
		mouseY(0),
		windowX(0),
		windowY(0),
		fps(0),
		frameCount(0),
		windowHandle(nullptr),
		cursorHandle(LoadCursor(nullptr, IDC_ARROW)),
		iconHandle(LoadIcon(nullptr, IDI_APPLICATION)),
		mouseCursorVisible(true),
		mouseCursorGrabbed(false),
		fullscreen(false),
		fpsLimit(Duration::fromSeconds(0.f)),
		delayWatch(Stopwatch::startNew()),
		fpsWatch(Stopwatch::startNew()),
		calcWatch(Stopwatch::startNew()),
		internalFrameCount(0),
		gctx(gctx)
	{
	}

	bool Window::enterFullscreen(const DisplayMode & displayMode)
	{
		/* create a DEVMODEA object to fill in */
		DEVMODEA fullscreenSettings = {};
		ZeroMemory(&fullscreenSettings, sizeof DEVMODEA);
		fullscreenSettings.dmSize = sizeof DEVMODEA;
		fullscreenSettings.dmDriverExtra = 0;

		/* enumerate the first displaysettings */
		BOOL success = EnumDisplaySettingsA(nullptr, 0, &fullscreenSettings);
		if (success == FALSE)
		{
			CORE_ERROR("Failed to enumerate the DisplaySettings at 0");
			return false;
		}
		
		/* copy the fields */
		fullscreenSettings.dmPelsWidth = displayMode.width;
		fullscreenSettings.dmPelsHeight = displayMode.height;
		fullscreenSettings.dmBitsPerPel = displayMode.bitsPerPel;
		fullscreenSettings.dmDisplayFrequency = displayMode.displayFrequency;
		fullscreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

		success = ChangeDisplaySettingsA(&fullscreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
		if (success == FALSE)
		{
			CORE_ERROR("Failed to change the DisplaySettings into fullscreen mode");
			return false;
		}

		/* change the window styles */
		SetWindowLongPtrA(this->windowHandle, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
		SetWindowLongPtrA(this->windowHandle, GWL_STYLE, WS_POPUP | WS_VISIBLE);

		/* update the window position and size */
		SetWindowPos(this->windowHandle, nullptr, 0, 0, displayMode.width, displayMode.height, SWP_SHOWWINDOW);

		/* show the window maximized */
		ShowWindow(this->windowHandle, SW_MAXIMIZE);

		/* grab the cursor */
		this->setMouseCursorGrabbed(true);
		this->fullscreen = true;
		return true;
	}

	bool Window::exitFullscreen(u32_t width, u32_t height)
	{
		const BOOL success = ChangeDisplaySettingsA(nullptr, CDS_RESET) == DISP_CHANGE_SUCCESSFUL;
		if (success == FALSE)
		{
			CORE_ERROR("Failed to reset the display settings");
			return FALSE;
		}
		
		/* change the window styles */
		SetWindowLongPtrA(this->windowHandle, GWL_EXSTYLE, WS_EX_LEFT);
		SetWindowLongPtrA(this->windowHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		
		/* calculate center x, center y, width and height */
		RECT wndRect = { 0l, 0l, (LONG)width, (LONG)height };
		AdjustWindowRect(&wndRect, GetWindowLongA(this->windowHandle, GWL_STYLE), FALSE);
		const int cx = wndRect.right - wndRect.left;
		const int cy = wndRect.bottom - wndRect.top;
		const int x = GetSystemMetrics(SM_CXSCREEN) / 2 - cx / 2;
		const int y = GetSystemMetrics(SM_CYSCREEN) / 2 - cy / 2;

		/* change position and size of the window */
		SetWindowPos(this->windowHandle, nullptr, x, y, cx, cy, SWP_SHOWWINDOW);
		
		/* show the window as a restored one */
		ShowWindow(this->windowHandle, SW_RESTORE);

		/* release the cursor from being grabbed */
		this->setMouseCursorGrabbed(false);

		SendMessageA(this->windowHandle, WM_SETICON, ICON_SMALL, (LPARAM)this->iconHandle);
		SendMessageA(this->windowHandle, WM_SETICON, ICON_BIG, (LPARAM)this->iconHandle);

		this->fullscreen = false;
		return true;
	}

	void Window::setFramerateLimit(i32_t limit)
	{
		if (limit > 0)
		{
			this->fpsLimit = Duration::fromSeconds(1.f / (float)limit);
		} else
		{
			this->noFramerateLimit();
		}
	}

	void Window::noFramerateLimit()
	{
		this->fpsLimit = Duration::Zero;
	}

	bool Window::isOpen() const
	{
		return this->windowHandle != nullptr;
	}

	void Window::close()
	{
		SendMessageA(this->windowHandle, WM_CLOSE, 0, 0);
	}

	bool Window::isFullscreen() const
	{
		return this->fullscreen;
	}

	void Window::setTitle(const std::string & title)
	{
		if (this->title != title)
		{
			this->title = title;
			SetWindowTextA(this->windowHandle, title.c_str());
		}
	}

	const std::string & Window::getTitle() const
	{
		return this->title;
	}

	void Window::setSize(u32_t width, u32_t height)
	{
		RECT wndRect = { 0l, 0l, (LONG)width, (LONG)height };
		AdjustWindowRect(&wndRect, GetWindowLongA(this->windowHandle, GWL_STYLE), FALSE);
		const int cx = wndRect.right - wndRect.left;
		const int cy = wndRect.bottom - wndRect.top;
		SetWindowPos(this->windowHandle, nullptr, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);
	}

	UVector2 Window::getSize() const
	{
		RECT wndRect = {};
		GetClientRect(this->windowHandle, &wndRect);
		const int cx = wndRect.right - wndRect.left;
		const int cy = wndRect.bottom - wndRect.top;
		return UVector2(cx, cy);
	}

	void Window::setPosition(i32_t x, i32_t y)
	{
		SetWindowPos(this->windowHandle, nullptr, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		
		if (this->mouseCursorGrabbed)
			grabCursor(true);
	}

	IVector2 Window::getPosition() const
	{
		RECT wndRect = {};
		GetWindowRect(this->windowHandle, &wndRect);
		return IVector2(wndRect.left, wndRect.top);
	}

	void Window::setResizable(bool resizable)
	{
		LONG_PTR style = GetWindowLongPtrA(this->windowHandle, GWL_STYLE);
		if (resizable) style |= WS_SIZEBOX;
		else style &= ~WS_SIZEBOX;
		SetWindowLongPtrA(this->windowHandle, GWL_STYLE, style);
	}

	bool Window::isResizable() const
	{
		return GetWindowLongPtrA(this->windowHandle, GWL_STYLE) & WS_SIZEBOX;
	}

	void Window::setMaximizable(bool maximizable)
	{
		LONG_PTR style = GetWindowLongPtrA(this->windowHandle, GWL_STYLE);
		if (maximizable) style |= WS_MAXIMIZEBOX;
		else style &= ~WS_MAXIMIZEBOX;
		SetWindowLongPtrA(this->windowHandle, GWL_STYLE, style);
	}

	bool Window::isMaximizable() const
	{
		return GetWindowLongPtrA(this->windowHandle, GWL_STYLE) & WS_MAXIMIZEBOX;
	}

	void Window::setMinimizable(bool minimizable)
	{
		LONG_PTR style = GetWindowLongPtrA(this->windowHandle, GWL_STYLE);
		if (minimizable) style |= WS_MINIMIZEBOX;
		else style &= ~WS_MINIMIZEBOX;
		SetWindowLongPtrA(this->windowHandle, GWL_STYLE, style);
	}

	bool Window::isMinimizable() const
	{
		return GetWindowLongPtrA(this->windowHandle, GWL_STYLE) & WS_MINIMIZEBOX;
	}

	void Window::setClosable(bool closable)
	{
		HMENU menu = GetSystemMenu(this->windowHandle, FALSE);
		const UINT uEnable = MF_BYCOMMAND | (closable ? MF_ENABLED : (MF_DISABLED | MF_GRAYED));
		EnableMenuItem(menu, SC_CLOSE, uEnable);
	}

	bool Window::isClosable() const
	{
		HMENU menu = GetSystemMenu(this->windowHandle, FALSE);
		MENUITEMINFOA info = {};
		ZeroMemory(&info, sizeof MENUITEMINFOA);
		info.cbSize = sizeof MENUITEMINFOA;
		info.fMask = MIIM_STATE;
		GetMenuItemInfoA(menu, 0, FALSE, &info);
		return !(info.fState & MFS_ENABLED);
	}

	void Window::setMouseCursorVisible(bool visible)
	{
		this->mouseCursorVisible = visible;
		SetCursor(visible ? this->cursorHandle : nullptr);
	}

	bool Window::isMouseCursorVisible() const
	{
		return this->mouseCursorVisible;
	}

	void Window::recenter()
	{
		this->setPosition(
			GetSystemMetrics(SM_CXSCREEN) / 2 - this->width / 2,
			GetSystemMetrics(SM_CYSCREEN) / 2 - this->height / 2
		);
	}

	bool Window::setIcon(const std::string & filepath)
	{
		if (this->iconHandle != nullptr || filepath.empty())
			DestroyIcon(this->iconHandle);

		if (filepath.empty())
		{
			SendMessageA(this->windowHandle, WM_SETICON, ICON_SMALL, (LPARAM)nullptr);
			SendMessageA(this->windowHandle, WM_SETICON, ICON_BIG, (LPARAM)nullptr);
		} else
		{
			this->iconHandle = (Resourcehandle)LoadImageA(GetModuleHandleA(nullptr), filepath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED | LR_DEFAULTSIZE | LR_DEFAULTCOLOR);
			if (this->iconHandle == nullptr)
			{
				CORE_ERROR("Failed to load \"%s\"", filepath.c_str());
				return false;
			}

			SendMessageA(this->windowHandle, WM_SETICON, ICON_SMALL, (LPARAM)this->iconHandle);
			SendMessageA(this->windowHandle, WM_SETICON, ICON_BIG, (LPARAM)this->iconHandle);
		}

		return true;
	}

	bool Window::setIcon(SystemIcon icon)
	{
		if (this->iconHandle != nullptr)
			DestroyIcon(this->iconHandle);
		
		const LPWSTR code = MAKEINTRESOURCE((int)icon);
		this->iconHandle = LoadIcon(nullptr, code);

		if (this->iconHandle == nullptr)
		{
			CORE_ERROR("Failed to load (%d) from the system", (int)code);
			return false;
		}

		SendMessageA(this->windowHandle, WM_SETICON, ICON_SMALL, (LPARAM)this->iconHandle);
		SendMessageA(this->windowHandle, WM_SETICON, ICON_BIG, (LPARAM)this->iconHandle);

		return true;
	}

	bool Window::setCursor(const std::string & filepath)
	{
		if (this->cursorHandle != nullptr || filepath.empty())
			DestroyCursor(this->cursorHandle);

		this->cursorHandle = (Resourcehandle)LoadImageA(GetModuleHandleA(nullptr), filepath.c_str(), IMAGE_CURSOR, 0, 0, LR_SHARED | LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR);

		if (this->cursorHandle == nullptr)
		{
			CORE_ERROR("Failed to load \"%s\"", filepath.c_str());
			return false;
		}

		SendMessageA(this->windowHandle, WM_SETCURSOR, 0, (LPARAM)this->cursorHandle);

		return true;
	}

	bool Window::setCursor(SystemCursor cursor)
	{
		if (this->cursorHandle != nullptr)
			DestroyCursor(this->cursorHandle);

		const LPWSTR code = MAKEINTRESOURCE((int)cursor);
		this->cursorHandle = LoadCursor(nullptr, code);

		if (this->cursorHandle == nullptr)
		{
			CORE_ERROR("Failed to load (%d)", (int)code);
			return false;
		}

		SendMessageA(this->windowHandle, WM_SETCURSOR, 0, (LPARAM)this->cursorHandle);

		return true;
	}

	void Window::setVisible(bool visible)
	{
		ShowWindow(this->windowHandle, visible ? SW_SHOW : SW_HIDE);
	}

	bool Window::isVisible() const
	{
		return IsWindowVisible(this->windowHandle);
	}

	void Window::dispatchEvents() const
	{
		MSG msg = {};
		while (PeekMessageA(&msg, this->windowHandle, 0, 0, 0))
		{
			if (GetMessageA(&msg, this->windowHandle, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
			}
		}
	}

	void Window::handleFps()
	{
		this->internalFrameCount++;
		this->frameCount++;
		this->calculateFps();
		this->limitFps();
	}
	
	void Window::grabCursor(bool grabbed)
	{
		if (grabbed)
		{
			RECT rect = {};
			GetClientRect(this->windowHandle, &rect);
			MapWindowPoints(this->windowHandle, nullptr, reinterpret_cast<LPPOINT>(&rect), 2);
			ClipCursor(&rect);
		} else
		{
			ClipCursor(nullptr);
		}
	}

	void Window::setMouseCursorGrabbed(bool grabbed)
	{
		this->mouseCursorGrabbed = grabbed;
		this->grabCursor(this->mouseCursorGrabbed);
	}

	bool Window::isMouseCursorGrabbed() const
	{
		return this->mouseCursorGrabbed;
	}

	void Window::calculateFps()
	{
		if (this->delayWatch.getElapsedTime().toSeconds() > 0.25f)
		{
			this->fps = (i32_t)(float)(this->internalFrameCount / this->fpsWatch.getElapsedTime().toSeconds());
			this->internalFrameCount = 0u;
			this->fpsWatch.restart();
			this->delayWatch.restart();
		}
	}

	void Window::limitFps()
	{
		if (this->fpsLimit != Duration::Zero)
		{
			const Duration diff = this->fpsLimit - this->calcWatch.getElapsedTime();
			const i32_t milliseconds = diff.toMilliseconds();

			if (milliseconds > 0)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
			}

			this->calcWatch.restart();
		}
	}

}