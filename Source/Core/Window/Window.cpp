// 
// Window.cpp
// Core
// 
// Created by Felix Busch on 16.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
//

#include <Core/Window/Window.hpp>
#include <Core/System/Error.hpp>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// Globals
	/// 
	////////////////////////////////////////////////////////////
	inline static constexpr wchar_t LpszClassName[] = L"Core_Window_Class";

	////////////////////////////////////////////////////////////
	/// Static members
	/// 
	////////////////////////////////////////////////////////////
	uint32_t Window::WindowCount = 0;

	////////////////////////////////////////////////////////////
	/// \brief Implementation class for the win32 window API
	/// 
	////////////////////////////////////////////////////////////
	class Window::Impl
	{
	public:
		
		////////////////////////////////////////////////////////////
		/// \brief Default Constructor
		///
		///	\param window The window to operate on
		/// 
		////////////////////////////////////////////////////////////
		explicit Impl(Window* window):
			Window(window),
			Resizing(false),
			MouseInside(false),
			KeyRepeatEnabled(true),
			Surrogate(0),
			CursorVisible(true),
			CursorGrabbed(false),
			Handle(nullptr),
			Cursor(LoadCursor(nullptr, IDC_ARROW))
		{
		}

		////////////////////////////////////////////////////////////
		/// \brief Converts a windows key code to Core key code
		/// 
		////////////////////////////////////////////////////////////
		static KeyCode VirtualKeyCodeToCoreKeyCode(WPARAM virtualKey, LPARAM flags)
		{
			switch (virtualKey)
			{
				// Check the scancode to distinguish between left and right shift
				case VK_SHIFT:
				{
					static UINT lShift = MapVirtualKeyW(VK_LSHIFT, MAPVK_VK_TO_VSC);
					const UINT scancode = static_cast<UINT>((flags & (0xFF << 16)) >> 16);
					return scancode == lShift ? KeyCode::LShift : KeyCode::RShift;
				}

				// Check the "extended" flag to distinguish between left and right alt
				case VK_MENU: return (HIWORD(flags) & KF_EXTENDED) ? KeyCode::RMenu : KeyCode::LMenu;

				// Check the "extended" flag to distinguish between left and right control
				case VK_CONTROL: return (HIWORD(flags) & KF_EXTENDED) ? KeyCode::RControl : KeyCode::LControl;

				case VK_CANCEL: return KeyCode::Cancel;
				case VK_BACK: return KeyCode::Back;
				case VK_TAB:return KeyCode::Tab;
				case VK_CLEAR: return KeyCode::Clear;
				case VK_RETURN: return KeyCode::Return;
				case VK_PAUSE: return KeyCode::Pause;
				case VK_CAPITAL: return KeyCode::Capital;
				case VK_KANA: return KeyCode::Kana;
				case VK_JUNJA: return KeyCode::Junja;
				case VK_FINAL: return KeyCode::Final;
				case VK_HANJA: return KeyCode::Hanja;
				case VK_ESCAPE: return KeyCode::Escape;
				case VK_CONVERT: return KeyCode::Convert;
				case VK_NONCONVERT: return KeyCode::NonConvert;
				case VK_ACCEPT: return KeyCode::Accept;
				case VK_MODECHANGE: return KeyCode::ModeChange;
				case VK_SPACE: return KeyCode::Space;
				case VK_PRIOR: return KeyCode::Prior;
				case VK_NEXT: return KeyCode::Next;
				case VK_END: return KeyCode::End;
				case VK_HOME: return KeyCode::Home;
				case VK_LEFT: return KeyCode::Left;
				case VK_UP: return KeyCode::Up;
				case VK_RIGHT: return KeyCode::Right;
				case VK_DOWN: return KeyCode::Down;
				case VK_SELECT: return KeyCode::Select;
				case VK_PRINT: return KeyCode::Print;
				case VK_EXECUTE: return KeyCode::Execute;
				case VK_SNAPSHOT: return KeyCode::Snapshot;
				case VK_INSERT: return KeyCode::Insert;
				case VK_DELETE: return KeyCode::Delete;
				case VK_HELP: return KeyCode::Help;
				case '0': return KeyCode::Num0;
				case '1': return KeyCode::Num1;
				case '2': return KeyCode::Num2;
				case '3': return KeyCode::Num3;
				case '4': return KeyCode::Num4;
				case '5': return KeyCode::Num5;
				case '6': return KeyCode::Num6;
				case '7': return KeyCode::Num7;
				case '8': return KeyCode::Num8;
				case '9': return KeyCode::Num9;
				case 'A': return KeyCode::A;
				case 'B': return KeyCode::B;
				case 'C': return KeyCode::C;
				case 'D': return KeyCode::D;
				case 'E': return KeyCode::E;
				case 'F': return KeyCode::F;
				case 'G': return KeyCode::G;
				case 'H': return KeyCode::H;
				case 'I': return KeyCode::I;
				case 'J': return KeyCode::J;
				case 'K': return KeyCode::K;
				case 'L': return KeyCode::L;
				case 'M': return KeyCode::M;
				case 'N': return KeyCode::N;
				case 'O': return KeyCode::O;
				case 'P': return KeyCode::P;
				case 'Q': return KeyCode::Q;
				case 'R': return KeyCode::R;
				case 'S': return KeyCode::S;
				case 'T': return KeyCode::T;
				case 'U': return KeyCode::U;
				case 'V': return KeyCode::V;
				case 'W': return KeyCode::W;
				case 'X': return KeyCode::X;
				case 'Y': return KeyCode::Y;
				case 'Z': return KeyCode::Z;
				case VK_LWIN: return KeyCode::LWin;
				case VK_RWIN: return KeyCode::RWin;
				case VK_APPS: return KeyCode::Apps;
				case VK_SLEEP: return KeyCode::Sleep;
				case VK_NUMPAD0: return KeyCode::Numpad0;
				case VK_NUMPAD1: return KeyCode::Numpad1;
				case VK_NUMPAD2: return KeyCode::Numpad2;
				case VK_NUMPAD3: return KeyCode::Numpad3;
				case VK_NUMPAD4: return KeyCode::Numpad4;
				case VK_NUMPAD5: return KeyCode::Numpad5;
				case VK_NUMPAD6: return KeyCode::Numpad6;
				case VK_NUMPAD7: return KeyCode::Numpad7;
				case VK_NUMPAD8: return KeyCode::Numpad8;
				case VK_NUMPAD9: return KeyCode::Numpad9;
				case VK_MULTIPLY: return KeyCode::Multiply;
				case VK_ADD: return KeyCode::Add;
				case VK_SEPARATOR: return KeyCode::Separator;
				case VK_SUBTRACT: return KeyCode::Subtract;
				case VK_DECIMAL: return KeyCode::Decimal;
				case VK_DIVIDE: return KeyCode::Divide;
				case VK_F1: return KeyCode::F1;
				case VK_F2: return KeyCode::F2;
				case VK_F3: return KeyCode::F3;
				case VK_F4: return KeyCode::F4;
				case VK_F5: return KeyCode::F5;
				case VK_F6: return KeyCode::F6;
				case VK_F7: return KeyCode::F7;
				case VK_F8: return KeyCode::F8;
				case VK_F9: return KeyCode::F9;
				case VK_F10: return KeyCode::F10;
				case VK_F11: return KeyCode::F11;
				case VK_F12: return KeyCode::F12;
				case VK_F13: return KeyCode::F13;
				case VK_F14: return KeyCode::F14;
				case VK_F15: return KeyCode::F15;
				case VK_F16: return KeyCode::F16;
				case VK_F17: return KeyCode::F17;
				case VK_F18: return KeyCode::F18;
				case VK_F19: return KeyCode::F19;
				case VK_F20: return KeyCode::F20;
				case VK_F21: return KeyCode::F21;
				case VK_F22: return KeyCode::F22;
				case VK_F23: return KeyCode::F23;
				case VK_F24: return KeyCode::F24;
				case VK_NUMLOCK: return KeyCode::Numlock;
				case VK_SCROLL: return KeyCode::Scroll;
				case VK_BROWSER_BACK: return KeyCode::BrowserBack;
				case VK_BROWSER_FORWARD: return KeyCode::BrowserForward;
				case VK_BROWSER_REFRESH: return KeyCode::BrowserRefresh;
				case VK_BROWSER_STOP: return KeyCode::BrowserStop;
				case VK_BROWSER_SEARCH: return KeyCode::BrowserSearch;
				case VK_BROWSER_FAVORITES: return KeyCode::BrowserFavorites;
				case VK_BROWSER_HOME: return KeyCode::BrowserHome;
				case VK_VOLUME_MUTE: return KeyCode::VolumeMute;
				case VK_VOLUME_DOWN: return KeyCode::VolumeDown;
				case VK_VOLUME_UP: return KeyCode::VolumeUp;
				case VK_MEDIA_NEXT_TRACK: return KeyCode::MediaNextTrack;
				case VK_MEDIA_PREV_TRACK: return KeyCode::MediaPrevTrack;
				case VK_MEDIA_STOP: return KeyCode::MediaStop;
				case VK_MEDIA_PLAY_PAUSE: return KeyCode::MediaPlayPause;
				case VK_LAUNCH_MAIL: return KeyCode::LaunchMail;
				case VK_LAUNCH_MEDIA_SELECT: return KeyCode::LaunchMediaSelect;
				case VK_LAUNCH_APP1: return KeyCode::LaunchApp1;
				case VK_LAUNCH_APP2: return KeyCode::LaunchApp2;
				case VK_OEM_1: return KeyCode::Oem1;
				case VK_OEM_PLUS: return KeyCode::OemPlus;
				case VK_OEM_COMMA: return KeyCode::OemComma;
				case VK_OEM_MINUS: return KeyCode::OemMinus;
				case VK_OEM_PERIOD: return KeyCode::OemPeriod;
				case VK_OEM_2: return KeyCode::Oem2;
				case VK_OEM_3: return KeyCode::Oem3;
				case VK_OEM_4: return KeyCode::Oem4;
				case VK_OEM_5: return KeyCode::Oem5;
				case VK_OEM_6: return KeyCode::Oem6;
				case VK_OEM_7: return KeyCode::Oem7;
				case VK_OEM_8: return KeyCode::Oem8;
				case VK_OEM_102: return KeyCode::Oem102;
				case VK_PROCESSKEY: return KeyCode::ProcessKey;
				case VK_PACKET: return KeyCode::Packet;
				case VK_ATTN: return KeyCode::Attn;
				case VK_CRSEL: return KeyCode::Crsel;
				case VK_EXSEL: return KeyCode::Exsel;
				case VK_EREOF: return KeyCode::Ereof;
				case VK_PLAY: return KeyCode::Play;
				case VK_ZOOM: return KeyCode::Zoom;
				case VK_NONAME: return KeyCode::NoName;
				case VK_PA1: return KeyCode::PA1;
				case VK_OEM_CLEAR: return KeyCode::OemClear;
				default: return KeyCode::Unknown;
			}
		}

		////////////////////////////////////////////////////////////
		/// \brief Perform event translation to custom event types
		///
		///	\param uMsg		win32 message id
		///	\param wParam	additional information about the event
		///	\param lParam	additional information about the event
		/// 
		////////////////////////////////////////////////////////////
		LResult ProcessEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			// notify any event
			WindowEvent generic = WindowEvent::Generic;
			generic.Any.Handle = Handle;
			generic.Any.Message = uMsg;
			generic.Any.WParam = wParam;
			generic.Any.LParam = lParam;
			Window->PushEvent(generic);
			
			switch (uMsg)
			{
				// The window is requested to be closed
				case WM_CLOSE:
				{
					// push close event
					Window->PushEvent(WindowEvent::Closed);
				} break;

				// the window should be destroyed
				case WM_DESTROY:
				{
					// maybe this isn't necessary but we'll do it for safety
					PostQuitMessage(EXIT_SUCCESS);

					// decrease window counter
					--WindowCount;

					// un-register the window class
					if(WindowCount == 0)
					{
						UnregisterClassW(LpszClassName, GetModuleHandle(nullptr));
					}
				} break;
				
				// The user started resizing the window
				case WM_ENTERSIZEMOVE:
				{
					Resizing = true;
					GrabCursor(false);
				} break;

				// the user has resized the window
				case WM_SIZE:
				{
					// get the current size
					const UInt2 size = Window->GetSize();
					
					const bool minimized   = wParam == SIZE_MINIMIZED;
					const bool isResizing  = Resizing;
					const bool sizeChanged = LastSize != size;
					
					if(!minimized && !isResizing && sizeChanged)
					{
						// copy the new size
						LastSize = size;
					
						// push a resize event
						WindowEvent event = WindowEvent::Resized;
						event.Size.Width  = size.X;
						event.Size.Height = size.Y;
						Window->PushEvent(event);

						GrabCursor(CursorGrabbed);
					}
				} break;

				// The user stopped resizing the window
				case WM_EXITSIZEMOVE:
				{
					// no longer resizing
					Resizing = false;
					
					// make sure the size has changed since the last resize
					if(const UInt2 size = Window->GetSize(); size != LastSize)
					{
						// copy the new size
						LastSize = size;
					
						// push a resize event
						WindowEvent event = WindowEvent::Resized;
						event.Size.Width  = size.X;
						event.Size.Height = size.Y;
						Window->PushEvent(event);
					}

					GrabCursor(CursorGrabbed);
				} break;

				// the vertical mouse wheel has been scrolled
				case WM_MOUSEWHEEL:
				{
					WindowEvent event       = WindowEvent::MouseWheelScrolled;
					event.MouseWheel.Wheel  = MouseWheel::Vertical;
					event.MouseWheel.Delta  = (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
					event.MouseWheel.MouseX = GET_X_LPARAM(lParam);
					event.MouseWheel.MouseY = GET_Y_LPARAM(lParam);
					Window->PushEvent(event);
				} break;

				// the horizontal mouse wheel has been scrolled
				case WM_MOUSEHWHEEL:
				{
					WindowEvent event       = WindowEvent::MouseWheelScrolled;
					event.MouseWheel.Wheel  = MouseWheel::Horizontal;
					event.MouseWheel.Delta  = (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
					event.MouseWheel.MouseX = GET_X_LPARAM(lParam);
					event.MouseWheel.MouseY = GET_Y_LPARAM(lParam);
					Window->PushEvent(event);
				} break;

				// the mouse has been moved
				case WM_MOUSEMOVE:
				{
					// extract the mouse local coordinates
					const int32_t x = GET_X_LPARAM(lParam);
					const int32_t y = GET_Y_LPARAM(lParam);

					// get the client area of the window
					RECT area;
					GetClientRect(Handle, &area);
					
					// capture the mouse in case the user wants to drag it outside
					if((wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON | MK_XBUTTON1 | MK_XBUTTON2)) == 0)
					{
						// only release the capture if we really have it
						if (GetCapture() == Handle)
							ReleaseCapture();
					} else if(GetCapture() != Handle)
					{
						// set the capture to continue receiving mouse events
						SetCapture(Handle);
					}

					// if the cursor is outside the client area
					if(x < area.left || x > area.right || y < area.top || area.bottom)
					{
						// and it used to be inside, the mouse left it
						if(MouseInside)
						{
							MouseInside = false;

							// no longer care for the mouse leaving the window
							TRACKMOUSEEVENT evt
							{
								.cbSize = sizeof(TRACKMOUSEEVENT),
								.dwFlags = TME_CANCEL,
								.hwndTrack = Handle,
								.dwHoverTime = HOVER_DEFAULT
							};

							TrackMouseEvent(&evt);

							// generate MouseLeft event
							Window->PushEvent(WindowEvent::MouseLeft);
						}
					} else
					{
						// and vice-versa
						if(!MouseInside)
						{
							MouseInside = true;

							// look for the mouse leaving the window
							// no longer care for the mouse leaving the window
							TRACKMOUSEEVENT evt
							{
								.cbSize = sizeof(TRACKMOUSEEVENT),
								.dwFlags = TME_HOVER,
								.hwndTrack = Handle,
								.dwHoverTime = HOVER_DEFAULT
							};

							TrackMouseEvent(&evt);

							// generate MouseEntered event
							Window->PushEvent(WindowEvent::MouseEntered);
						}
					}

					// generate a MouseMove event
					WindowEvent event = WindowEvent::MouseMoved;
					event.MouseMove.MouseX = x;
					event.MouseMove.MouseY = y;
					Window->PushEvent(event);
				} break;

				// the left mouse button has been pressed
				case WM_LBUTTONDOWN:
				{
					WindowEvent event = WindowEvent::MousePressed;
					event.MouseButton.Button = MouseButton::Left;
					event.MouseButton.MouseX = GET_X_LPARAM(lParam);
					event.MouseButton.MouseY = GET_Y_LPARAM(lParam);
					Window->PushEvent(event);
				} break;

				// the left mouse button has been released
				case WM_LBUTTONUP:
				{
					WindowEvent event = WindowEvent::MouseReleased;
					event.MouseButton.Button = MouseButton::Left;
					event.MouseButton.MouseX = GET_X_LPARAM(lParam);
					event.MouseButton.MouseY = GET_Y_LPARAM(lParam);
					Window->PushEvent(event);
				} break;

				// the right mouse button has been pressed
				case WM_RBUTTONDOWN:
				{
					WindowEvent event = WindowEvent::MousePressed;
					event.MouseButton.Button = MouseButton::Right;
					event.MouseButton.MouseX = GET_X_LPARAM(lParam);
					event.MouseButton.MouseY = GET_Y_LPARAM(lParam);
					Window->PushEvent(event);
				} break;

				// the right mouse button has been released
				case WM_RBUTTONUP:
				{
					WindowEvent event = WindowEvent::MouseReleased;
					event.MouseButton.Button = MouseButton::Right;
					event.MouseButton.MouseX = GET_X_LPARAM(lParam);
					event.MouseButton.MouseY = GET_Y_LPARAM(lParam);
					Window->PushEvent(event);
				} break;

				// A x-button mouse button has been pressed
				case WM_XBUTTONDOWN:
				{
					WindowEvent event = WindowEvent::MousePressed;
					event.MouseButton.Button = GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? MouseButton::XButton1 : MouseButton::XButton2;
					event.MouseButton.MouseX = GET_X_LPARAM(lParam);
					event.MouseButton.MouseY = GET_Y_LPARAM(lParam);
					Window->PushEvent(event);
				} break;

				// A x-button mouse button has been released
				case WM_XBUTTONUP:
				{
					WindowEvent event = WindowEvent::MouseReleased;
					event.MouseButton.Button = GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? MouseButton::XButton1 : MouseButton::XButton2;
					event.MouseButton.MouseX = GET_X_LPARAM(lParam);
					event.MouseButton.MouseY = GET_Y_LPARAM(lParam);
					Window->PushEvent(event);
				} break;

				// the OS wants to know the smallest and largest window size
				case WM_GETMINMAXINFO:
				{
					if(auto* info = reinterpret_cast<MINMAXINFO*>(lParam))
					{
						info->ptMaxTrackSize.x = 50000;
						info->ptMaxTrackSize.y = 50000;
					}
				} break;

				// a key has been pressed
				case WM_SYSKEYDOWN:
				case WM_KEYDOWN:
				{
					if(KeyRepeatEnabled || (HIWORD(lParam) & KF_REPEAT) == 0)
					{
						WindowEvent event  = WindowEvent::KeyReleased;
						event.Key.Code     = VirtualKeyCodeToCoreKeyCode(wParam, lParam);
						event.Key.Alt      = HIWORD(GetKeyState(VK_MENU)) != 0;
						event.Key.Control  = HIWORD(GetKeyState(VK_CONTROL)) != 0;
						event.Key.Shift    = HIWORD(GetKeyState(VK_SHIFT)) != 0;
						event.Key.System   = HIWORD(GetKeyState(VK_LWIN)) || HIWORD(GetKeyState(VK_RWIN)) != 0;
						Window->PushEvent(event);
					}
				} break;

				// a key has been released
				case WM_SYSKEYUP:
				case WM_KEYUP:
				{
					WindowEvent event  = WindowEvent::KeyReleased;
					event.Key.Code     = VirtualKeyCodeToCoreKeyCode(wParam, lParam);
					event.Key.Alt      = HIWORD(GetKeyState(VK_MENU)) != 0;
					event.Key.Control  = HIWORD(GetKeyState(VK_CONTROL)) != 0;
					event.Key.Shift    = HIWORD(GetKeyState(VK_SHIFT)) != 0;
					event.Key.System   = HIWORD(GetKeyState(VK_LWIN)) || HIWORD(GetKeyState(VK_RWIN)) != 0;
					Window->PushEvent(event);
				} break;

				// text entered event
				case WM_CHAR:
				{
					if(KeyRepeatEnabled || (lParam & (1 << 30)) == 0)
					{
						// get the code of the typed character
						auto character = (uint32_t)wParam;

						const bool highSurrogate = character >= 0xD800 && character <= 0xDBFF;
						const bool lowSurrogate  = character >= 0xDC00 && character <= 0xDFFF;

						// check if it is the first part of a surrogate pair, or a regular character
						if(highSurrogate)
						{
							// first part of a surrogate pair: store it and wait for the second one
							Surrogate = (uint16_t)character;
						} else
						{
							// check if it is the second part of a surrogate pair, or a regular character
							if(lowSurrogate)
							{
								// https://unicodebook.readthedocs.io/unicode_encodings.html
								// convert 16-bit surrogate pair to utf32 character
								uint32_t unicode = USHRT_MAX + 1;
								unicode += (Surrogate & 0x03FF) << 10;
								unicode += character & 0x03FF;
								
								// the second element is valid: convert the two elements to a utf32 character
								character = unicode;
								Surrogate = 0;
							}
						}

						WindowEvent event = WindowEvent::TextEntered;
						event.Text.Unicode = character;
						Window->PushEvent(event);
					}
				} break;

				// the window lost focus
				case WM_KILLFOCUS:
				{
					Window->PushEvent(WindowEvent::FocusLost);
					GrabCursor(false);
				} break;

				// the window gained focus
				case WM_SETFOCUS:
				{
					Window->PushEvent(WindowEvent::FocusGained);
					GrabCursor(CursorGrabbed);
				} break;
				
				// we don't care
				default:
				{
					if (uMsg == WM_SETCURSOR)
					{
						if (LOWORD(lParam) == HTCLIENT)
						{
							SetCursor(CursorVisible ? Cursor : nullptr);
							return TRUE;
						}
					}

					return DefWindowProcW(Handle, uMsg, wParam, lParam);
				}
			}

			return 0;
		}

		////////////////////////////////////////////////////////////
		/// \brief Windows event callback function
		/// 
		////////////////////////////////////////////////////////////
		static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			if (uMsg == WM_CREATE)
			{
				// extract creation structure
				if (const auto* pcs = reinterpret_cast<LPCREATESTRUCTW>(lParam))
				{
					// get the user data
					if (const auto userData = reinterpret_cast<LONG_PTR>(pcs->lpCreateParams))
					{
						// set user data
						SetWindowLongPtrW(hWnd, GWLP_USERDATA, userData);

						// increase window counter
						++WindowCount;

						// tell the OS about the success
						return 0;
					}
				}

				// if this happens, the window gets destroyed immediately since anything went wrong
				return -1;
			}

			// get the user data
			const LONG_PTR userData = GetWindowLongPtrW(hWnd, GWLP_USERDATA);
			
			// cast to impl instance
			if(auto* impl = reinterpret_cast<Impl*>(userData); impl && impl->Handle)
			{
				return impl->ProcessEvent(uMsg, wParam, lParam);
			}

			return DefWindowProcW(hWnd, uMsg, wParam, lParam);
		}

		////////////////////////////////////////////////////////////
		/// \brief Clip/Free the mouse cursor inside the window boundary
		/// 
		////////////////////////////////////////////////////////////
		void GrabCursor(bool grab) const
		{
			if (grab)
			{
				// get the window boundary
				RECT rect;
				if(!GetClientRect(Handle, &rect))
				{
					Err() << "Failed to get the client rectangle" << std::endl;
					return;
				}

				// convert the rectangle to the top, left and bottom, right coordinate
				MapWindowPoints(Handle, nullptr, reinterpret_cast<LPPOINT>(&rect), 2);
				if(!ClipCursor(&rect))
				{
					Err() << "Failed to grab the cursor" << std::endl;
				}
			}
			else
			{
				if(!ClipCursor(nullptr))
				{
					Err() << "Failed to let the cursor be free" << std::endl;
				}
			}
		}

	public:

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		Window*			Window;				//!< The window to notify on event
		bool			Resizing;			//!< Is the window currently being resized?
		bool			MouseInside;		//!< Is the mouse currently inside the window boundary?
		bool			KeyRepeatEnabled;	//!< Send multiple keyboard event when holding a key down
		uint16_t		Surrogate;			//!< The first part surrogate pair when text is entered
		bool			CursorVisible;		//!< The visibility of the cursor
		bool			CursorGrabbed;		//!< The grab state of the cursor inside the window boundary
		WindowHandle	Handle;				//!< The window Handle to interact with win32
		CursorHandle	Cursor;				//!< The cursor to use when he's visible
		UInt2			LastSize;			//!< The size before resizing

	};

	////////////////////////////////////////////////////////////
	Window::Window():
		impl(new Impl(this))
	{
	}

	////////////////////////////////////////////////////////////
	Window::~Window()
	{
		// make sure to really destroy the window on destruction
		if (IsOpen())
		{
			Destroy();
		}
	}

	////////////////////////////////////////////////////////////
	bool Window::Open(int32_t width, int32_t height, const String& title)
	{
		// prevent registering the window class more than once
		if(WindowCount == 0)
		{
			InitWindowClass();
		}

		// define window styles
		constexpr DWORD dwStyle		= WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		constexpr DWORD dwExStyle	= WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

		// get real window size including borders
		RECT windowArea = { 0l, 0l, (LONG)width, (LONG)height };
		if(!AdjustWindowRectEx(&windowArea, dwStyle, FALSE, dwExStyle))
		{
			// print failure info
			Err() << "Failed to adjust window rectangle" << std::endl;
		}

		// compute real window size
		const int32_t realWidth  = (int32_t)(windowArea.right - windowArea.left);
		const int32_t realHeight = (int32_t)(windowArea.bottom - windowArea.top);

		// get monitor device context
		const HDC screenDc = GetDC(nullptr);
		
		// get x, y coordinate on the monitor
		const int32_t left = (GetDeviceCaps(screenDc, HORZRES) - realWidth) / 2;
		const int32_t top  = (GetDeviceCaps(screenDc, VERTRES) - realHeight) / 2;

		// free monitor device context
		ReleaseDC(nullptr, screenDc);
		
		// create the window
		impl->Handle = CreateWindowExW(dwExStyle, LpszClassName, title.c_str(), dwStyle, left, top, width, height, nullptr, nullptr, GetModuleHandle(nullptr), impl.get());

		// validate success
		if(impl->Handle == nullptr)
		{
			Err() << "Failed to create the window" << std::endl;
			return false;
		}

		// send Resize event
		SetSize(width, height);

		return true;
	}

	////////////////////////////////////////////////////////////
	void Window::Close()
	{
		// send close event to callback
		if(!PostMessageW(impl->Handle, WM_CLOSE, 0, 0))
		{
			Err() << "Failed to post a close event" << std::endl;
		}
	}

	////////////////////////////////////////////////////////////
	void Window::Destroy()
	{
		// send destroy event to callback
		if(!DestroyWindow(impl->Handle))
		{
			Err() << "Failed to destroy the window" << std::endl;
		}
		
		impl->Handle = nullptr;
	}

	////////////////////////////////////////////////////////////
	bool Window::IsOpen() const
	{
		return impl->Handle != nullptr;
	}

	////////////////////////////////////////////////////////////
	bool Window::IsFullscreenWindow() const
	{
		// get the fullscreen boundary
		RECT fullscreenBounds = {};
		if(!GetWindowRect(GetDesktopWindow(), &fullscreenBounds))
		{
			Err() << "Failed to get the desktop window boundary" << std::endl;
			return false;
		}

		// get the application boundary
		RECT appBounds = {};
		if(!GetWindowRect(impl->Handle, &appBounds))
		{
			Err() << "Failed to get the application boundary" << std::endl;
			return false;
		}

		// compare boundaries
		return (fullscreenBounds.left	== appBounds.left	&&
				fullscreenBounds.top	== appBounds.top	&&
				fullscreenBounds.right	== appBounds.right	&&
				fullscreenBounds.bottom == appBounds.bottom);
	}

	////////////////////////////////////////////////////////////
	void Window::SetSize(uint32_t width, uint32_t height)
	{
		// get the current window styles
		const auto dwStyle = (DWORD)GetWindowLongW(impl->Handle, GWL_STYLE);
		const auto dwExStyle = (DWORD)GetWindowLongW(impl->Handle, GWL_EXSTYLE);

		// compute real dimensions including the borders
		RECT windowArea = { 0l, 0l, (LONG)width, (LONG)height };
		if(!AdjustWindowRectEx(&windowArea, dwStyle, FALSE, dwExStyle))
		{
			Err() << "Failed to set the window size" << std::endl;
		}

		// compute the real width and height
		const auto w = (int32_t)(windowArea.right - windowArea.left);
		const auto h = (int32_t)(windowArea.bottom - windowArea.top);

		// update the size
		if(!SetWindowPos(impl->Handle, nullptr, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER))
		{
			Err() << "Failed to set the window size" << std::endl;
		}
	}

	////////////////////////////////////////////////////////////
	void Window::SetSize(const UInt2& size)
	{
		SetSize(size.X, size.Y);
	}

	////////////////////////////////////////////////////////////
	UInt2 Window::GetSize() const
	{
		// get the window size
		RECT windowBoundary = {};
		if(!GetClientRect(impl->Handle, &windowBoundary))
		{
			Err() << "Failed to retrieve the window size" << std::endl;
			return { 0u, 0u };
		}

		return
		{
			(uint32_t)(windowBoundary.right - windowBoundary.left),
			(uint32_t)(windowBoundary.bottom - windowBoundary.top)
		};
	}

	////////////////////////////////////////////////////////////
	void Window::SetPosition(int32_t x, int32_t y)
	{
		if(!SetWindowPos(impl->Handle, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER))
		{
			Err() << "Failed to set the window position";
		}

		impl->GrabCursor(impl->CursorGrabbed);
	}

	////////////////////////////////////////////////////////////
	Int2 Window::GetPosition() const
	{
		RECT windowBoundary = {};
		if(!GetWindowRect(impl->Handle, &windowBoundary))
		{
			Err() << "Failed to retrieve the window boundary";
		}

		return { windowBoundary.left, windowBoundary.top };
	}

	////////////////////////////////////////////////////////////
	void Window::SetTitle(const String& title)
	{
		SetWindowTextW(impl->Handle, title.c_str());
	}

	////////////////////////////////////////////////////////////
	String Window::GetTitle() const
	{
		// receive the length of the title
		const int titleLength = GetWindowTextLengthW(impl->Handle);

		// declare string buffer
		String buffer;
		buffer.resize((size_t)titleLength);

		// write the title into buffer
		GetWindowTextW(impl->Handle, &buffer[0], titleLength);

		return buffer;
	}

	////////////////////////////////////////////////////////////
	void Window::SetKeyRepeatEnabled(bool enabled)
	{
		impl->KeyRepeatEnabled = enabled;
	}

	////////////////////////////////////////////////////////////
	bool Window::IsKeyRepeatEnabled() const
	{
		return impl->KeyRepeatEnabled;
	}

	////////////////////////////////////////////////////////////
	void Window::SetMaximizeButtonEnabled(bool enabled)
	{
		SetWindowStyle(WS_MAXIMIZEBOX, enabled);
	}

	////////////////////////////////////////////////////////////
	bool Window::IsMaximizeButtonEnabled() const
	{
		return IsWindowStyleEnabled(WS_MAXIMIZEBOX);
	}

	////////////////////////////////////////////////////////////
	void Window::SetMinimizeButtonEnabled(bool enabled)
	{
		SetWindowStyle(WS_MINIMIZEBOX, enabled);
	}

	////////////////////////////////////////////////////////////
	bool Window::IsMinimizeButtonEnabled() const
	{
		return IsWindowStyleEnabled(WS_MINIMIZEBOX);
	}

	////////////////////////////////////////////////////////////
	void Window::SetCloseButtonEnabled(bool enabled)
	{
		static constexpr auto Enable = MF_BYCOMMAND | MF_ENABLED;
		static constexpr auto Disable = MF_BYCOMMAND | MF_GRAYED | MF_DISABLED;
		EnableMenuItem(GetSystemMenu(impl->Handle, FALSE), SC_CLOSE, (UINT)(enabled ? Enable : Disable));
	}

	////////////////////////////////////////////////////////////
	bool Window::IsCloseButtonEnabled() const
	{
		MENUITEMINFOW info;
		info.cbSize = sizeof(MENUITEMINFOW);
		info.fMask = MIIM_STATE;
		GetMenuItemInfoW(GetSystemMenu(impl->Handle, FALSE), SC_CLOSE, FALSE, &info);
		return !(info.fState & MFS_DISABLED);
	}

	////////////////////////////////////////////////////////////
	void Window::SetResizable(bool enabled)
	{
		SetWindowStyle(WS_THICKFRAME, enabled);
	}

	////////////////////////////////////////////////////////////
	bool Window::IsResizable() const
	{
		return IsWindowStyleEnabled(WS_THICKFRAME);
	}

	////////////////////////////////////////////////////////////
	void Window::SetVisible(bool visible)
	{
		ShowWindow(impl->Handle, visible ? SW_SHOW : SW_HIDE);
	}

	////////////////////////////////////////////////////////////
	bool Window::IsVisible() const
	{
		return IsWindowVisible(impl->Handle);
	}

	////////////////////////////////////////////////////////////
	void Window::SetMouseCursorVisible(bool visible)
	{
		impl->CursorVisible = visible;
		SetCursor(visible ? impl->Cursor : nullptr);
	}

	////////////////////////////////////////////////////////////
	bool Window::IsMouseCursorVisible() const
	{
		return impl->CursorVisible;
	}

	////////////////////////////////////////////////////////////
	void Window::SetMouseCursorGrabbed(bool grabbed)
	{
		impl->CursorGrabbed = grabbed;
		impl->GrabCursor(grabbed);
	}

	////////////////////////////////////////////////////////////
	bool Window::IsMouseCursorGrabbed()
	{
		return impl->CursorGrabbed;
	}

	////////////////////////////////////////////////////////////
	void Window::RequestFocus()
	{
		// get the process ids
		const DWORD thisPid = GetWindowThreadProcessId(impl->Handle, nullptr);
		const DWORD foregroundPid = GetWindowThreadProcessId(GetForegroundWindow(), nullptr);

		// make sure we only set the focus if we really can
		if(thisPid == foregroundPid)
		{
			SetFocus(impl->Handle);
		} else
		{
			// generate flash event in taskbar
			FLASHWINFO flash
			{
				sizeof(FLASHWINFO),
				impl->Handle,
				FLASHW_TRAY,
				3,
				0
			};

			FlashWindowEx(&flash);
		}
	}

	////////////////////////////////////////////////////////////
	bool Window::HasFocus() const
	{
		return GetForegroundWindow() == impl->Handle;
	}

	////////////////////////////////////////////////////////////
	void Window::SetMouseCursor(const MouseCursor& cursor)
	{
		this->cursor = cursor;
		impl->Cursor = cursor.GetCursorHandle();
		SetCursor(impl->CursorVisible ? impl->Cursor : nullptr);
	}

	////////////////////////////////////////////////////////////
	const MouseCursor& Window::GetMouseCursor() const
	{
		return cursor;
	}

	////////////////////////////////////////////////////////////
	WindowHandle Window::GetWindowHandle() const
	{
		return impl->Handle;
	}

	////////////////////////////////////////////////////////////
	void Window::InitWindowClass() const
	{
		const WNDCLASSEXW wcex
		{
			.cbSize = sizeof(WNDCLASSEXW),
			.style = CS_HREDRAW | CS_VREDRAW,
			.lpfnWndProc = &Impl::WinProc,
			.cbClsExtra = 0,
			.cbWndExtra = 0,
			.hInstance = GetModuleHandle(nullptr),
			.hIcon = nullptr,
			.hCursor = nullptr,
			.hbrBackground = nullptr,
			.lpszMenuName = nullptr,
			.lpszClassName = LpszClassName,
			.hIconSm = nullptr
		};

		RegisterClassExW(&wcex);
	}

	////////////////////////////////////////////////////////////
	void Window::SetWindowStyle(int64_t style, bool enabled)
	{
		LONG currentStyle = GetWindowLongW(impl->Handle, GWL_STYLE);

		if (enabled) currentStyle |= (LONG)style;
		else currentStyle &= ~(LONG)style;

		SetWindowLongW(impl->Handle, GWL_STYLE, currentStyle);
	}

	////////////////////////////////////////////////////////////
	bool Window::IsWindowStyleEnabled(int64_t style) const
	{
		return GetWindowLongW(impl->Handle, GWL_STYLE) & style;
	}

	////////////////////////////////////////////////////////////
	void Window::QueueEvents()
	{
		// message buffer
		MSG msg = {};

		// keep going as long as there are events to process
		while(PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// translate & dispatch messages to notify the Impl::WinProc method
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

}