#include <Core/Window/Window.hpp>

#include <windows.h>
#include <windowsx.h>

#include <iostream>
#include <thread>

#include <Core/Window/DisplayMode.hpp>

namespace Core
{

	const i32_t Window::displayWidth = DisplayMode::getDesktopMode().width;
	const i32_t Window::displayHeight = DisplayMode::getDesktopMode().height;

	Window::Window() :
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
		mouseInsideWindow(false),
		open(false),
		fpsLimit(Duration::fromSeconds(0.f)),
		delayWatch(Stopwatch::startNew()),
		fpsWatch(Stopwatch::startNew()),
		calcWatch(Stopwatch::startNew()),
		internalFrameCount(0)
	{
	}

	Window::~Window()
	{
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

	bool Window::create(u32_t width, u32_t height, const std::string & title)
	{
		WNDCLASSA wc = {};
		ZeroMemory(&wc, sizeof WNDCLASSA);
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		wc.hCursor = this->cursorHandle;
		wc.hIcon = this->iconHandle;
		wc.hInstance = GetModuleHandleA(nullptr);
		wc.lpfnWndProc = &Window::processEvents;
		wc.lpszClassName = LPSZ_CLASS_NAME;
		wc.lpszMenuName = nullptr;
		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

		if (!RegisterClassA(&wc))
		{
			std::cerr << "Failed to register the window class" << std::endl;
			return false;
		}

		const DWORD dwStyle = WS_OVERLAPPEDWINDOW;
		RECT wndRect = { 0l, 0l, (LONG)width, (LONG)height };
		AdjustWindowRect(&wndRect, dwStyle, FALSE);
		const int nWidth  = wndRect.right - wndRect.left;
		const int nHeight = wndRect.bottom - wndRect.top;
		const int x = GetSystemMetrics(SM_CXSCREEN) / 2 - nWidth  / 2;
		const int y = GetSystemMetrics(SM_CYSCREEN) / 2 - nHeight / 2;

		this->windowHandle = CreateWindowA(wc.lpszClassName, title.c_str(), dwStyle, x, y, nWidth, nHeight, nullptr, nullptr, wc.hInstance, this);
		if (this->windowHandle == nullptr)
		{
			std::cerr << "Failed to create a window" << std::endl;
			return false;
		}

		ShowWindow(this->windowHandle, SW_SHOW);
		UpdateWindow(this->windowHandle);
		SetForegroundWindow(this->windowHandle);

		this->title = title;
		this->open = true;

		return true;
	}

	bool Window::isOpen() const
	{
		return this->open;
	}

	void Window::close()
	{
		SendMessageA(this->windowHandle, WM_CLOSE, 0, 0);
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
				std::cerr << "Failed to load \"" << filepath << "\"" << std::endl;
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
			std::cerr << "Failed to load (" << (int)icon << ") from the system" << std::endl;
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
			std::cerr << "Failed to load \"" << filepath << "\"" << std::endl;
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
			std::cerr << "Failed to load (" << (int)code << ")" << std::endl;
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
	
	i64_t __stdcall Window::processEvents(Windowhandle handle, u32_t msg, u64_t wParam, i64_t lParam)
	{
		static Window * window = nullptr;

		switch (msg)
		{
			case WM_CREATE:
			{
				const LPVOID creationParams = reinterpret_cast<CREATESTRUCTA *>(lParam)->lpCreateParams;
				window = reinterpret_cast<Window *>(creationParams);

				if (window == nullptr)
				{
					std::cerr << "failed to decode creation-parameters into a valid window" << std::endl;
				}
			} break;

			case WM_SETCURSOR:
			{
				if (LOWORD(lParam) == HTCLIENT)
				{
					SetCursor(window->mouseCursorVisible ? window->cursorHandle : nullptr);
					return TRUE;
				}
			} break;

			case WM_CLOSE:
			{
				// close the window
				window->open = false;
				window->onWindowClosed();
			} break;

			case WM_SIZE:
			{
				if (wParam != SIZE_MINIMIZED)
				{
					const u16_t width = GET_X_LPARAM(lParam);
					const u16_t height = GET_Y_LPARAM(lParam);

					const bool resized = window->width != width || window->height != height;
					if (resized)
					{
						window->width = (i32_t)width;
						window->height = (i32_t)height;
						window->onWindowResized();
					}
				}
			} break;

			case WM_MOVE:
			{
				const u16_t x = GET_X_LPARAM(lParam);
				const u16_t y = GET_Y_LPARAM(lParam);

				const bool windowMoved = window->windowX != x || window->windowY != y;
				if (windowMoved)
				{
					window->windowX = (i32_t)x;
					window->windowY = (i32_t)y;
					window->onWindowMoved();
				}
			} break;

			case WM_MOUSEWHEEL:
			{
				const u16_t delta = GET_WHEEL_DELTA_WPARAM(wParam);
				window->onMouseWheelScrolled(delta);
			} break;

			case WM_MOUSEMOVE:
			{
				const u16_t x = GET_X_LPARAM(lParam);
				const u16_t y = GET_Y_LPARAM(lParam);

				const bool mouseMoved = window->mouseX != x || window->mouseY != y;
				if (mouseMoved)
				{
					RECT wndRect = {};
					GetClientRect(handle, &wndRect);

					// If the cursor is outside the client area
					if ((x < wndRect.left) || (x > wndRect.right) || (y < wndRect.top) || (y > wndRect.bottom))
					{
						if (window->mouseInsideWindow)
						{
							window->mouseInsideWindow = false;

							// we don't care about the mouse leaving the window
							window->trackMouseEvent(false);

							// fire onMouseLeft event
							window->onMouseLeft();
						}
					} else // If the cursor is inside the client area
					{
						if (!window->mouseInsideWindow)
						{
							window->mouseInsideWindow = true;

							// we care about the mouse leaving the window
							window->trackMouseEvent(true);

							// fire onMouseEntered event
							window->onMouseEntered();
						}
					}

					window->pmouseX = window->mouseX;
					window->pmouseY = window->mouseY;

					window->mouseX = (i32_t)x;
					window->mouseY = (i32_t)y;
					window->onMouseMoved();
				}
			} break;

			case WM_MOUSELEAVE:
			{
				// Avoid the firing a second time in case the cursor is dragged outside
				if (window->mouseInsideWindow)
				{
					window->mouseInsideWindow = false;
					window->onMouseLeft();
				}
			} break;

			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				KeyboardEventArgs args;
				args.alt = (HIWORD(GetKeyState(VK_LMENU)) || HIWORD(GetKeyState(VK_RMENU))) != 0;
				args.control = (HIWORD(GetKeyState(VK_LCONTROL)) || HIWORD(GetKeyState(VK_RCONTROL))) != 0;
				args.shift = (HIWORD(GetKeyState(VK_LSHIFT)) || HIWORD(GetKeyState(VK_RSHIFT))) != 0;
				args.system = (HIWORD(GetKeyState(VK_LWIN)) || HIWORD(GetKeyState(VK_RWIN))) != 0;
				args.code = decodeKeyCode(wParam, lParam);
				window->onKeyPressed(args);
			} break;

			case WM_SYSKEYUP:
			case WM_KEYUP:
			{
				KeyboardEventArgs args;
				args.alt = (HIWORD(GetKeyState(VK_LMENU)) || HIWORD(GetKeyState(VK_RMENU))) != 0;
				args.control = (HIWORD(GetKeyState(VK_LCONTROL)) || HIWORD(GetKeyState(VK_RCONTROL))) != 0;
				args.shift = (HIWORD(GetKeyState(VK_LSHIFT)) || HIWORD(GetKeyState(VK_RSHIFT))) != 0;
				args.system = (HIWORD(GetKeyState(VK_LWIN)) || HIWORD(GetKeyState(VK_RWIN))) != 0;
				args.code = decodeKeyCode(wParam, lParam);
				window->onKeyReleased(args);
			} break;

			case WM_CHAR:
			{
				const Keyboard::Key key = (Keyboard::Key)wParam;
				window->onTextEntered(key);
			} break;

			case WM_LBUTTONDOWN: window->onMousePressed(Mouse::Button::Left); break;
			case WM_RBUTTONDOWN: window->onMousePressed(Mouse::Button::Right); break;
			case WM_MBUTTONDOWN: window->onMousePressed(Mouse::Button::Middle); break;
			case WM_XBUTTONDOWN: window->onMousePressed(GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? Mouse::Button::XButton1 : Mouse::Button::XButton2); break;
			case WM_LBUTTONUP: window->onMouseReleased(Mouse::Button::Left); break;
			case WM_RBUTTONUP: window->onMouseReleased(Mouse::Button::Right); break;
			case WM_MBUTTONUP: window->onMouseReleased(Mouse::Button::Middle); break;
			case WM_XBUTTONUP: window->onMouseReleased(GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? Mouse::Button::XButton1 : Mouse::Button::XButton2); break;
			case WM_SETFOCUS: window->onFocusGained(); break;
			case WM_KILLFOCUS: window->onFocusLost(); break;

			default: return DefWindowProcA(handle, msg, wParam, lParam);
		}

		return 0;
	}

	Keyboard::Key Window::decodeKeyCode(u64_t wParam, i64_t lParam)
	{
		WPARAM vk = (WPARAM)0;
		const UINT scancode = (lParam & 0x00FF0000) >> 16;
		const BOOL extended = (lParam & 0x01000000) != 0;

		switch (wParam)
		{
			case VK_SHIFT:		vk = MapVirtualKeyA(scancode, MAPVK_VSC_TO_VK_EX); break;
			case VK_CONTROL:	vk = extended ? VK_RCONTROL : VK_LCONTROL; break;
			case VK_MENU:		vk = extended ? VK_RMENU : VK_LMENU; break;
			default:			vk = wParam; break;
		}

		return static_cast<Keyboard::Key>(vk);
	}

	void Window::trackMouseEvent(bool active)
	{
		TRACKMOUSEEVENT tme = {};
		ZeroMemory(&tme, sizeof TRACKMOUSEEVENT);
		tme.cbSize = sizeof TRACKMOUSEEVENT;
		tme.dwFlags = active ? TME_LEAVE : TME_CANCEL;
		tme.hwndTrack = this->windowHandle;
		tme.dwHoverTime = HOVER_DEFAULT;
		TrackMouseEvent(&tme);
	}

	void Window::destroy()
	{
		this->trackMouseEvent(false);
		this->setMouseCursorVisible(true);
		
		if(this->windowHandle) DestroyWindow(this->windowHandle);
		if(this->cursorHandle) DestroyCursor(this->cursorHandle);
		if(this->iconHandle) DestroyIcon(this->iconHandle);
		UnregisterClassA(LPSZ_CLASS_NAME, GetModuleHandleA(NULL));
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