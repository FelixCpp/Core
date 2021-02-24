#include <Core/Rendering/RenderWindow.hpp> // Core::RenderWindow
#include <Core/Rendering/Renderers/Renderer.hpp> // Core::Renderer
#include <Core/Rendering/FactoryManager.hpp> // Core::FactoryManager
#include <Core/System/Logger.hpp> // CORE_ERROR

/// <summary>
/// Windows API
/// </summary>
#include <Windows.h>
#include <windowsx.h>

namespace
{
	// directly taken from: https://github.com/SFML/SFML/blob/master/include/SFML/System/Utf.inl
	static Core::u16_t surrogate = '\0';

	template <typename In>
	static In Utf16Decode(In begin, In end, Core::u32_t & output, Core::u32_t replacement)
	{
		Core::u16_t first = *begin++;

		// If it's a surrogate pair, first convert to a single UTF-32 character
		if ((first >= 0xD800) && (first <= 0xDBFF))
		{
			if (begin < end)
			{
				Core::u32_t second = *begin++;
				if ((second >= 0xDC00) && (second <= 0xDFFF))
				{
					// The second element is valid: convert the two elements to a UTF-32 character
					output = ((first - 0xD800) << 10) + (second - 0xDC00) + 0x0010000;
				} else
				{
					// Invalid character
					output = replacement;
				}
			} else
			{
				// Invalid character
				begin = end;
				output = replacement;
			}
		} else
		{
			// We can make a direct copy
			output = first;
		}

		return begin;
	}

	template <typename In, typename Out>
	static Out FromUtf16ToUtf32(In begin, In end, Out output)
	{
		while (begin < end)
		{
			Core::u32_t codepoint = 0u;
			begin = Utf16Decode(begin, end, codepoint, '?');
			*output++ = codepoint;
		}

		return output;
	}
}

namespace Core
{

	RenderWindow::RenderWindow(LateRef<Renderer> renderer, LateRef<RenderStateManager> rsm) :
		Window(),
		RenderTarget(renderer, rsm),
		FrameSaver(renderer, this->windowHandle),
		mouseInsideWindow(false),
		resizing(false),
		renderer(renderer)
	{
	}

	bool RenderWindow::Create(u32_t width, u32_t height, const std::string & title)
	{
		WNDCLASSA wc = {};
		ZeroMemory(&wc, sizeof WNDCLASSA);
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		wc.hCursor = this->cursorHandle;
		wc.hIcon = this->iconHandle;
		wc.hInstance = GetModuleHandleA(nullptr);
		wc.lpfnWndProc = &RenderWindow::ProcessEvents;
		wc.lpszClassName = LPSZ_CLASS_NAME;
		wc.lpszMenuName = nullptr;
		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

		if (!RegisterClassA(&wc))
		{
			CORE_ERROR("Failed to register the window class");
			return false;
		}

		const DWORD dwStyle = WS_OVERLAPPEDWINDOW;
		RECT wndRect = { 0l, 0l, (LONG)width, (LONG)height };
		AdjustWindowRect(&wndRect, dwStyle, FALSE);
		const int nWidth = wndRect.right - wndRect.left;
		const int nHeight = wndRect.bottom - wndRect.top;
		const int x = GetSystemMetrics(SM_CXSCREEN) / 2 - nWidth / 2;
		const int y = GetSystemMetrics(SM_CYSCREEN) / 2 - nHeight / 2;

		this->windowHandle = CreateWindowA(wc.lpszClassName, title.c_str(), dwStyle, x, y, nWidth, nHeight, nullptr, nullptr, wc.hInstance, this);
		if (this->windowHandle == nullptr)
		{
			CORE_ERROR("Failed to create a window");
			return false;
		}

		ShowWindow(this->windowHandle, SW_SHOW);
		UpdateWindow(this->windowHandle);
		SetForegroundWindow(this->windowHandle);

		this->title = title;

		return true;
	}

	i64_t __stdcall RenderWindow::ProcessEvents(Windowhandle handle, u32_t msg, u64_t wParam, i64_t lParam)
	{
		static RenderWindow * window = nullptr;

		switch (msg)
		{
			case WM_CREATE:
			{
				const LPVOID creationParams = reinterpret_cast<CREATESTRUCTA *>(lParam)->lpCreateParams;
				window = reinterpret_cast<RenderWindow *>(creationParams);

				if (window == nullptr)
				{
					CORE_ERROR("failed to decode creation-parameters into a valid window");
					break;
				}
			} break;

			case WM_SETCURSOR:
			{
				if (LOWORD(lParam) == HTCLIENT)
				{
					::SetCursor(window->IsMouseCursorVisible() ? window->cursorHandle : nullptr);
					return TRUE;
				}
			} break;

			case WM_CLOSE:
			{
				/* minimize the window */
				CloseWindow(handle);

				/* raise the onClosed() event */
				window->OnWindowClosed();

				/* Destroy the window, this call sends a message so WM_DESTROY gets called */
				DestroyWindow(handle);
			} break;

			case WM_DESTROY:
			{
				window->TrackMouseEvent(false);
				window->SetMouseCursorVisible(true);

				if (window->IsFullscreen())
				{
					ChangeDisplaySettingsA(nullptr, 0);
				}

				if (window->cursorHandle) DestroyCursor(window->cursorHandle);
				if (window->iconHandle) DestroyIcon(window->iconHandle);
				UnregisterClassA(LPSZ_CLASS_NAME, GetModuleHandleA(nullptr));
				window->windowHandle = nullptr;
			} break;

			case WM_SIZE:
			{
				if (wParam != SIZE_MINIMIZED && !window->resizing)
				{
					const u16_t width = GET_X_LPARAM(lParam);
					const u16_t height = GET_Y_LPARAM(lParam);

					const bool resized = window->width != width || window->height != height;
					if (resized)
					{
						window->width = (i32_t)width;
						window->height = (i32_t)height;
						window->OnWindowResized();

						/* resize the viewport */
						if (Renderer * renderer = window->renderer)
						{
							if (renderer->IsInitialized())
							{
								renderer->ResizeViewport(width, height);
							}
						}

						/* grab the cursor after resizing */
						window->GrabCursor(window->mouseCursorGrabbed);
					}
				}
			} break;

			// Start resizing
			case WM_ENTERSIZEMOVE:
			{
				window->resizing = true;
				window->GrabCursor(false);
			} break;

			// Stop resizing
			case WM_EXITSIZEMOVE:
			{
				window->resizing = false;

				// Ignore cases where the window has only been moved
				const UVector2 size = window->GetSize();
				if (window->width != size.width || window->height != size.height)
				{
					// Update the last handled size
					window->width = size.width;
					window->height = size.height;

					/* resize the viewport */
					if (Renderer * renderer = window->renderer)
					{
						if (renderer->IsInitialized())
						{
							renderer->ResizeViewport(size.width, size.height);
						}
					}

					// Push a resize event
					window->OnWindowResized();
				}

				// Restore/update cursor grabbing
				window->GrabCursor(window->mouseCursorGrabbed);
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
					window->OnWindowMoved();
				}
			} break;

			case WM_MOUSEWHEEL:
			{
				const u16_t delta = GET_WHEEL_DELTA_WPARAM(wParam);
				window->OnMouseWheelScrolled(delta);
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
							window->TrackMouseEvent(false);

							// fire onMouseLeft event
							window->OnMouseLeft();
						}
					} else // If the cursor is inside the client area
					{
						if (!window->mouseInsideWindow)
						{
							window->mouseInsideWindow = true;

							// we care about the mouse leaving the window
							window->TrackMouseEvent(true);

							// fire onMouseEntered event
							window->OnMouseEntered();
						}
					}

					window->pmouseX = window->mouseX;
					window->pmouseY = window->mouseY;

					window->mouseX = (i32_t)x;
					window->mouseY = (i32_t)y;
					window->OnMouseMoved();
				}
			} break;

			case WM_MOUSELEAVE:
			{
				// Avoid the firing a second time in case the cursor is dragged outside
				if (window->mouseInsideWindow)
				{
					window->mouseInsideWindow = false;
					window->OnMouseLeft();
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
				args.code = DecodeKeyCode(wParam, lParam);
				window->OnKeyPressed(args);

				window->keyIsPressed = true;
			} break;

			case WM_SYSKEYUP:
			case WM_KEYUP:
			{
				KeyboardEventArgs args;
				args.alt = (HIWORD(GetKeyState(VK_LMENU)) || HIWORD(GetKeyState(VK_RMENU))) != 0;
				args.control = (HIWORD(GetKeyState(VK_LCONTROL)) || HIWORD(GetKeyState(VK_RCONTROL))) != 0;
				args.shift = (HIWORD(GetKeyState(VK_LSHIFT)) || HIWORD(GetKeyState(VK_RSHIFT))) != 0;
				args.system = (HIWORD(GetKeyState(VK_LWIN)) || HIWORD(GetKeyState(VK_RWIN))) != 0;
				args.code = DecodeKeyCode(wParam, lParam);
				window->OnKeyReleased(args);

				window->keyIsPressed = false;
			} break;

			case WM_GETMINMAXINFO:
			{
				// We override the returned information to remove the default limit
				// (the OS doesn't allow windows bigger than the desktop by default)
				MINMAXINFO * info = reinterpret_cast<MINMAXINFO *>(lParam);
				info->ptMaxTrackSize.x = 50000;
				info->ptMaxTrackSize.y = 50000;
			} break;

			case WM_CHAR:
			{
				// directly taken from: https://github.com/SFML/SFML/blob/master/src/SFML/Window/Win32/WindowImplWin32.cpp
				if (window->keyRepeatEnabled || ((lParam & (1 << 30)) == 0))
				{
					// Get the code of the typed character
					u32_t character = static_cast<u32_t>(wParam);

					// Check if it is the first part of a surrogate pair, or a regular character
					if ((character >= 0xD800) && (character <= 0xDBFF))
					{
						// First part of a surrogate pair: store it and wait for the second one
						surrogate = static_cast<u16_t>(character);
					} else
					{
						// Check if it is the second part of a surrogate pair, or a regular character
						if ((character >= 0xDC00) && (character <= 0xDFFF))
						{
							// Convert the UTF-16 surrogate pair to a single UTF-32 value
							u16_t utf16[] = { surrogate, static_cast<u16_t>(character) };
							FromUtf16ToUtf32(utf16, utf16 + 2, &character);
							surrogate = 0;
						}

						// Send a TextEntered event
						window->OnTextEntered(character);
					}
				}
			} break;

			case WM_LBUTTONDOWN: window->mouseIsPressed = true; window->OnMousePressed(Mouse::Button::Left); break;
			case WM_RBUTTONDOWN: window->mouseIsPressed = true; window->OnMousePressed(Mouse::Button::Right); break;
			case WM_MBUTTONDOWN: window->mouseIsPressed = true; window->OnMousePressed(Mouse::Button::Middle); break;
			case WM_XBUTTONDOWN: window->mouseIsPressed = true; window->OnMousePressed(GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? Mouse::Button::XButton1 : Mouse::Button::XButton2); break;
			case WM_LBUTTONUP: window->mouseIsPressed = false; window->OnMouseReleased(Mouse::Button::Left); break;
			case WM_RBUTTONUP: window->mouseIsPressed = false; window->OnMouseReleased(Mouse::Button::Right); break;
			case WM_MBUTTONUP: window->mouseIsPressed = false; window->OnMouseReleased(Mouse::Button::Middle); break;
			case WM_XBUTTONUP: window->mouseIsPressed = false; window->OnMouseReleased(GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? Mouse::Button::XButton1 : Mouse::Button::XButton2); break;
			case WM_SETFOCUS: window->GrabCursor(window->mouseCursorGrabbed); window->OnFocusGained(); break;
			case WM_KILLFOCUS: window->GrabCursor(false); window->OnFocusLost(); break;

			default: return DefWindowProcA(handle, msg, wParam, lParam);
		}

		return 0;
	}

	Keyboard::Key RenderWindow::DecodeKeyCode(u64_t wParam, i64_t lParam)
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

	void RenderWindow::TrackMouseEvent(bool active)
	{
		TRACKMOUSEEVENT tme = {};
		ZeroMemory(&tme, sizeof TRACKMOUSEEVENT);
		tme.cbSize = sizeof TRACKMOUSEEVENT;
		tme.dwFlags = active ? TME_LEAVE : TME_CANCEL;
		tme.hwndTrack = this->windowHandle;
		tme.dwHoverTime = HOVER_DEFAULT;
		::TrackMouseEvent(&tme);
	}

}