// 
// Types.hpp
// Core
// 
// Created by Felix Busch on 19.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <cstdint>

////////////////////////////////////////////////////////////
/// Types
/// 
////////////////////////////////////////////////////////////
extern "C" {
	struct HWND__;
	struct HICON__;
}

namespace Core
{
	////////////////////////////////////////////////////////////
	/// Type definitions
	/// 
	////////////////////////////////////////////////////////////
	using WindowHandle = HWND__*;
	using CursorHandle = HICON__*;
	using IconHandle   = HICON__*;

#ifdef _WIN64
	using MessageID = uint32_t;
	using WParam = uint64_t;
	using LParam = int64_t;
	using LResult = int64_t;
#elif defined(_WIN32)
	using MessageID = uint32_t;
	using WParam = uint32_t;
	using LParam = long;
	using LResult = long;
#endif

}