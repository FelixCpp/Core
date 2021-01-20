#include <Core/Rendering/RenderWindow.hpp> // Core::RenderWindow
#include <Core/Rendering/Renderers/Renderer.hpp> // Core::Renderer
#include <Core/Rendering/FactoryManager.hpp> // Core::FactoryManager
#include <Core/System/Logger.hpp> // CORE_ERROR


/// <summary>
/// Windows API
/// </summary>
#include <Windows.h>
#include <windowsx.h>

namespace Core
{

	RenderWindow::RenderWindow(Renderer *& renderer, RenderStateManager *& rsm) :
		Window(),
		RenderTarget(renderer, rsm),
		mouseInsideWindow(false),
		resizing(false),
		renderer(renderer)
	{
	}

	RenderWindow::~RenderWindow()
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

				// Initialize the factories
				if (!FactoryManager::Initialize())
				{
					break;
				}

				// initialize the renderer
				if (!window->renderer->Initialize(handle))
				{
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

				/* destroy Direct2D */
				//window->renderer->Destroy();

				FactoryManager::Destroy();

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
						if (window->renderer)
						{
							window->renderer->ResizeViewport(width, height);
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
					if (window->renderer)
					{
						window->renderer->ResizeViewport(size.width, size.height);
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
				const Keyboard::Key key = (Keyboard::Key)wParam;
				window->OnTextEntered(key);
			} break;

			case WM_LBUTTONDOWN: window->OnMousePressed(Mouse::Button::Left); break;
			case WM_RBUTTONDOWN: window->OnMousePressed(Mouse::Button::Right); break;
			case WM_MBUTTONDOWN: window->OnMousePressed(Mouse::Button::Middle); break;
			case WM_XBUTTONDOWN: window->OnMousePressed(GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? Mouse::Button::XButton1 : Mouse::Button::XButton2); break;
			case WM_LBUTTONUP: window->OnMouseReleased(Mouse::Button::Left); break;
			case WM_RBUTTONUP: window->OnMouseReleased(Mouse::Button::Right); break;
			case WM_MBUTTONUP: window->OnMouseReleased(Mouse::Button::Middle); break;
			case WM_XBUTTONUP: window->OnMouseReleased(GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? Mouse::Button::XButton1 : Mouse::Button::XButton2); break;
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