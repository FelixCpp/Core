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

	typedef u32_t color_t;

	typedef HWND__ * Windowhandle;
	typedef HICON__ * Resourcehandle;
	
	
}