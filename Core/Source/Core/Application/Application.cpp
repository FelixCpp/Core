#include <Core/Application/Application.hpp>
#include <Core/Application/GraphicsContext.hpp>

#include <windows.h>
#include <iostream>

#include <thread>

namespace Core
{

	Application::Application(i32_t width, i32_t height, const std::string & title) :
		RenderTarget(&this->gctx),
		width(0), height(0),
		windowX(0), windowY(0),
		mouseX(0), mouseY(0),
		pmouseX(0), pmouseY(0),
		windowHandle(nullptr),
		cursorHandle(LoadCursor(nullptr, IDC_ARROW)),
		iconHandle(LoadIcon(nullptr, IDI_APPLICATION)),
		isOpen(false),
		drawingPaused(false),
		mouseInsideWindow(false),
		mouseCursorVisible(true),
		lpszClassName("Core Window"),
		gctx(nullptr),
		fpsLimit(),
		delayWatch(Stopwatch::startNew()),
		fpsWatch(Stopwatch::startNew()),
		calcWatch(Stopwatch::startNew())
	{
		this->createWindow(width, height, title);
	}

	Application::~Application()
	{
		if (this->gctx)
		{
			delete this->gctx;
			this->gctx = nullptr;
		}

		if (this->iconHandle) DestroyIcon(this->iconHandle);
		if (this->cursorHandle) DestroyCursor(this->cursorHandle);
		if (this->windowHandle) DestroyWindow(this->windowHandle);
		UnregisterClassA(this->lpszClassName, GetModuleHandleA(NULL));
	}

	void Application::pauseDrawing()
	{
		this->drawingPaused = true;
	}

	void Application::startDrawing()
	{
		this->drawingPaused = false;
	}

	void Application::frameRate(i32_t fpsLimit)
	{
		this->fpsLimit = Duration::fromSeconds(1.f / (float)fpsLimit);
	}

	void Application::setSize(u32_t width, u32_t height)
	{
		RECT wndRect = { 0l, 0l, (LONG)width, (LONG)height };
		AdjustWindowRect(&wndRect, GetWindowLongA(this->windowHandle, GWL_STYLE), FALSE);
		const i32_t w = wndRect.right - wndRect.left;
		const i32_t h = wndRect.bottom - wndRect.top;
		SetWindowPos(this->windowHandle, NULL, 0, 0, w, h, SWP_NOZORDER | SWP_NOMOVE);
	}

	void Application::setPosition(i32_t x, i32_t y)
	{
		SetWindowPos(this->windowHandle, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	void Application::setTitle(const std::string & title)
	{
		if (this->title != title)
		{
			this->title = title;
			SetWindowTextA(this->windowHandle, title.c_str());
		}
	}

	void Application::recenter()
	{
		const i32_t displayWidth  = GetSystemMetrics(SM_CXSCREEN);
		const i32_t displayHeight = GetSystemMetrics(SM_CYSCREEN);
		this->setPosition(displayWidth / 2 - this->width / 2, displayHeight / 2 - this->height / 2);
	}

	bool Application::setIcon(const std::string & filepath)
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

	bool Application::setIcon(SystemIcon icon)
	{
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

	void Application::setMouseCursorVisible(bool visible)
	{
		this->mouseCursorVisible = visible;
		SetCursor(visible ? this->cursorHandle : nullptr);
	}

	void Application::exit()
	{
		this->isOpen = false;
	}

	bool Application::setCursor(const std::string & filepath)
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

	bool Application::setCursor(SystemCursor cursor)
	{
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

	void Application::setupImpl()
	{
		this->gctx->beginDraw();
		this->setup();
		this->gctx->endDraw();
	}

	void Application::drawImpl()
	{
		this->gctx->beginDraw();
		this->draw();
		this->gctx->endDraw();
	}

	void Application::calculateFrameRate()
	{
		if (this->delayWatch.getElapsedTime().toSeconds() >= 0.25f)
		{
			this->fps = (i32_t)(float)(this->frameCount / this->fpsWatch.getElapsedTime().toSeconds());
			this->frameCount = 0u;
			this->fpsWatch.restart();
			this->delayWatch.restart();
		}
	}

	void Application::limitFrameRate()
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

	void Application::createWindow(i32_t width, i32_t height, const std::string & title)
	{
		WNDCLASSA wc = {};
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		wc.hCursor = this->cursorHandle;
		wc.hIcon = this->iconHandle;
		wc.hInstance = GetModuleHandleA(NULL);
		wc.lpfnWndProc = &Application::handleEvents;
		wc.lpszClassName = this->lpszClassName;
		wc.lpszMenuName = NULL;
		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		
		if (!RegisterClassA(&wc))
		{
			std::cerr << "RegisterClassA call failed" << std::endl;
			return;
		}
		
		const DWORD dwStyle = WS_SYSMENU;
		const i32_t x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
		const i32_t y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;
		this->windowHandle = CreateWindowA(this->lpszClassName, "", WS_SYSMENU, x, y, width, height, NULL, NULL, GetModuleHandleA(NULL), this);

		if (this->windowHandle == NULL)
		{
			std::cerr << "CreateWindowA call failed" << std::endl;
			return;
		}

		ShowWindow(this->windowHandle, SW_SHOW);
		UpdateWindow(this->windowHandle);
		SetForegroundWindow(this->windowHandle);

		this->setSize(width, height);
		this->setTitle(title);

		this->isOpen = true;
	}

	void Application::startSketch()
	{
		this->frameRate(60);
		this->push();
		
		this->setupImpl();
		this->startDrawing();

		while (this->isOpen)
		{
			if (!this->drawingPaused)
				this->drawImpl();

			// update frameRate
			this->frameCount++;
			this->calculateFrameRate();
			this->limitFrameRate();

			this->dispatchEvents();
		}
	}

	void Application::dispatchEvents()
	{
		MSG msg = {};
		while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	void Application::trackMouseEvent(bool active)
	{
		TRACKMOUSEEVENT tme = {};
		ZeroMemory(&tme, sizeof TRACKMOUSEEVENT);
		tme.cbSize = sizeof TRACKMOUSEEVENT;
		tme.dwFlags = active ? TME_LEAVE : TME_CANCEL;
		tme.hwndTrack = this->windowHandle;
		tme.dwHoverTime = HOVER_DEFAULT;
		TrackMouseEvent(&tme);
	}

	Keyboard::Key Application::decodeKeyCode(u64_t wParam, i64_t lParam)
	{
		WPARAM vk = (WPARAM)0;
		const UINT scancode = (lParam & 0x00FF0000) >> 16;
		const BOOL extended = (lParam & 0x01000000) != 0;

		switch (wParam)
		{
			case VK_SHIFT: vk = MapVirtualKeyA(scancode, MAPVK_VSC_TO_VK_EX); break;
			case VK_CONTROL: vk = extended ? VK_RCONTROL : VK_LCONTROL; break;
			case VK_MENU: vk = extended ? VK_RMENU : VK_LMENU; break;
			default: vk = wParam; break;
		}

		return static_cast<Keyboard::Key>(vk);
	}

	i64_t __stdcall Application::handleEvents(Windowhandle handle, u32_t msg, u64_t wParam, i64_t lParam)
	{
		static Application * app = nullptr;

		switch (msg)
		{
			case WM_CREATE:
			{
				const LPVOID creationParams = reinterpret_cast<CREATESTRUCTA *>(lParam)->lpCreateParams;
				app = reinterpret_cast<Application *>(creationParams);

				if (app == nullptr)
				{
					std::cerr << "failed to decode creation-parameters into a valid window" << std::endl;
				}
				else
				{
					// create graphics-context
					if (!app->gctx)
					{
						app->gctx = new GraphicsContext(handle);
					}
				}
			} break;

			case WM_SETCURSOR:
			{
				if (LOWORD(lParam) == HTCLIENT)
				{
					SetCursor(app->mouseCursorVisible ? app->cursorHandle : nullptr);
					return TRUE;
				}
			} break;

			case WM_CLOSE:
			{
				// release graphics-context
				if (app->gctx)
				{
					delete app->gctx;
					app->gctx = nullptr;
				}

				// close the window
				app->isOpen = false;
				app->onWindowClosed();
			} break;

			case WM_DESTROY:
			{
				// cleanup
				app->trackMouseEvent(false);
				app->setMouseCursorVisible(true);
				UnregisterClassA(app->lpszClassName, GetModuleHandleA(NULL));
				CloseWindow(handle);
				DestroyWindow(handle);
				PostQuitMessage(EXIT_SUCCESS);
			} break;

			case WM_SIZE:
			{
				if (wParam != SIZE_MINIMIZED)
				{
					const u16_t width  = LOWORD(lParam);
					const u16_t height = HIWORD(lParam);
					
					const bool resized = app->width != width || app->height != height;
					if (resized)
					{
						app->gctx->resizeViewport(width, height);

						app->width  = (i32_t)width;
						app->height = (i32_t)height;
						app->onWindowResized();
					}
				}
			} break;

			case WM_MOVE:
			{
				const u16_t x = LOWORD(lParam);
				const u16_t y = HIWORD(lParam);

				const bool windowMoved = app->windowX != x || app->windowY != y;
				if (windowMoved)
				{
					app->windowX = (i32_t)x;
					app->windowY = (i32_t)y;
					app->onWindowMoved();
				}
			} break;

			case WM_MOUSEWHEEL:
			{
				const u16_t delta = HIWORD(wParam);
				app->onMouseWheelScrolled(delta);
			} break;

			case WM_MOUSEMOVE:
			{
				const u16_t x = LOWORD(lParam);
				const u16_t y = HIWORD(lParam);

				const bool mouseMoved = app->mouseX != x || app->mouseY != y;
				if (mouseMoved)
				{
					RECT wndRect = {};
					GetClientRect(handle, &wndRect);

					// If the cursor is outside the client area
					if ((x < wndRect.left) || (x > wndRect.right) || (y < wndRect.top) || (y > wndRect.bottom))
					{
						if (app->mouseInsideWindow)
						{
							app->mouseInsideWindow = false;
							
							// we don't care about the mouse leaving the window
							app->trackMouseEvent(false);

							// fire onMouseLeft event
							app->onMouseLeft();
						}
					} else // If the cursor is inside the client area
					{
						if (!app->mouseInsideWindow)
						{
							app->mouseInsideWindow = true;

							// we care about the mouse leaving the window
							app->trackMouseEvent(true);

							// fire onMouseEntered event
							app->onMouseEntered();
						}
					}
					
					app->pmouseX = app->mouseX;
					app->pmouseY = app->mouseY;

					app->mouseX = (i32_t)x;
					app->mouseY = (i32_t)y;
					app->onMouseMoved();
				}
			} break;

			case WM_MOUSELEAVE:
			{
				// Avoid the firing a second time in case the cursor is dragged outside
				if (app->mouseInsideWindow)
				{
					app->mouseInsideWindow = false;
					app->onMouseLeft();
				}
			} break;

			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				KeyboardEventArgs args;
				args.alt     = (HIWORD(GetKeyState(VK_LMENU))    || HIWORD(GetKeyState(VK_RMENU)))    != 0;
				args.control = (HIWORD(GetKeyState(VK_LCONTROL)) || HIWORD(GetKeyState(VK_RCONTROL))) != 0;
				args.shift   = (HIWORD(GetKeyState(VK_LSHIFT))   || HIWORD(GetKeyState(VK_RSHIFT)))   != 0;
				args.system  = (HIWORD(GetKeyState(VK_LWIN))     || HIWORD(GetKeyState(VK_RWIN)))     != 0;
				args.code    = decodeKeyCode(wParam, lParam);
				app->onKeyPressed(args);
			} break;

			case WM_SYSKEYUP:
			case WM_KEYUP:
			{
				KeyboardEventArgs args;
				args.alt     = (HIWORD(GetKeyState(VK_LMENU))    || HIWORD(GetKeyState(VK_RMENU)))    != 0;
				args.control = (HIWORD(GetKeyState(VK_LCONTROL)) || HIWORD(GetKeyState(VK_RCONTROL))) != 0;
				args.shift   = (HIWORD(GetKeyState(VK_LSHIFT))   || HIWORD(GetKeyState(VK_RSHIFT)))   != 0;
				args.system  = (HIWORD(GetKeyState(VK_LWIN))     || HIWORD(GetKeyState(VK_RWIN)))     != 0;
				args.code    = decodeKeyCode(wParam, lParam);
				app->onKeyReleased(args);
			} break;

			case WM_CHAR:
			{
				const Keyboard::Key key = (Keyboard::Key)wParam;
				app->onTextEntered(key);
			} break;

			case WM_LBUTTONDOWN: app->onMousePressed(Mouse::Button::Left); break;
			case WM_RBUTTONDOWN: app->onMousePressed(Mouse::Button::Right); break;
			case WM_MBUTTONDOWN: app->onMousePressed(Mouse::Button::Middle); break;
			case WM_XBUTTONDOWN: app->onMousePressed(HIWORD(wParam) == XBUTTON1 ? Mouse::Button::XButton1 : Mouse::Button::XButton2); break;
			case WM_LBUTTONUP: app->onMouseReleased(Mouse::Button::Left); break;
			case WM_RBUTTONUP: app->onMouseReleased(Mouse::Button::Right); break;
			case WM_MBUTTONUP: app->onMouseReleased(Mouse::Button::Middle); break;
			case WM_XBUTTONUP: app->onMouseReleased(HIWORD(wParam) == XBUTTON1 ? Mouse::Button::XButton1 : Mouse::Button::XButton2); break;
			case WM_SETFOCUS: app->onFocusGained(); break;
			case WM_KILLFOCUS: app->onFocusLost(); break;
			
			default: return DefWindowProcA(handle, msg, wParam, lParam);
		}

		return 0;
	}

}