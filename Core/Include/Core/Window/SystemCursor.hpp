#pragma once

namespace Core
{

	enum class SystemCursor {
		Arrow		= 0x7F00,
		IBeam		= 0x7F01,
		Wait		= 0x7F02,
		Cross		= 0x7F03,
		UpArrow		= 0x7F04,
		Size		= 0x7F80,
		Icon		= 0x7F81,
		SizeNWSE	= 0x7F82,
		SizeNESW	= 0x7F83,
		SizeWE		= 0x7F84,
		SizeNS		= 0x7F85,
		SizeAll		= 0x7F86,
		No			= 0x7F88,
		Hand		= 0x7F89,
		AppStarting = 0x7F8A,
		Help		= 0x7F8B,
	};

}