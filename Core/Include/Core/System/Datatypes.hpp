#pragma once

struct HWND__;
struct HICON__;

namespace Core
{

	typedef signed __int8 i8_t;
	typedef unsigned __int8 u8_t;

	typedef signed __int16 i16_t;
	typedef unsigned __int16 u16_t;

	typedef signed __int32 i32_t;
	typedef unsigned __int32 u32_t;

	typedef signed __int64 i64_t;
	typedef unsigned __int64 u64_t;

	/* used for Window creation */
	typedef HWND__ * Windowhandle;

	/* used for Window-Cursor/Window-Icon creation */
	typedef HICON__ * Resourcehandle;
	
}