#pragma once

struct HWND__;
struct HICON__;

namespace Core
{

	typedef signed char i8_t;
	typedef unsigned char u8_t;

	typedef short i16_t;
	typedef unsigned short u16_t;

	typedef int i32_t;
	typedef unsigned int u32_t;

	typedef long long i64_t;
	typedef unsigned long long u64_t;

	/* used for Window creation */
	typedef HWND__ * Windowhandle;

	/* used for Window-Cursor/Window-Icon creation */
	typedef HICON__ * Resourcehandle;
	
}