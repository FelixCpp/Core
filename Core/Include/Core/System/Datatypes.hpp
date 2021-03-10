#pragma once

struct HWND__;
struct HICON__;

namespace Core
{

	typedef signed char Int8;
	typedef unsigned char UInt8;

	typedef short Int16;
	typedef unsigned short UInt16;

	typedef int Int32;
	typedef unsigned int UInt32;

	typedef long long Int64;
	typedef unsigned long long UInt64;

	/* used for Window creation */
	typedef HWND__ * Windowhandle;

	/* used for Window-Cursor/Window-Icon creation */
	typedef HICON__ * Resourcehandle;
	
}