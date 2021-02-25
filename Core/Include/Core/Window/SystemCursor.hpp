#pragma once

namespace Core
{

	enum class SystemCursor {
		Arrow		= 0x7F00, //< Standard arrow
		IBeam		= 0x7F01, //< I-beam
		Wait		= 0x7F02, //< Hourglass
		Cross		= 0x7F03, //< Crosshair
		UpArrow		= 0x7F04, //< Vertical arrow
		Size		= 0x7F80, //< Obsolete for applications marked version 4.0 or later. Use IDC_SIZEALL.
		Icon		= 0x7F81, //< Obsolete for applications marked version 4.0 or later.
		SizeNWSE	= 0x7F82, //< Double-pointed arrow pointing northwest and southeast
		SizeNESW	= 0x7F83, //< Double-pointed arrow pointing northeast and southwest
		SizeWE		= 0x7F84, //< Double-pointed arrow pointing west and east
		SizeNS		= 0x7F85, //< Double-pointed arrow pointing north and south
		SizeAll		= 0x7F86, //< Four-pointed arrow pointing north, south, east, and west
		No			= 0x7F88, //< Slashed circle
		Hand		= 0x7F89, //< Hand
		AppStarting = 0x7F8A, //< Standard arrow and small hourglass
		Help		= 0x7F8B, //< Arrow and question mark
	};

}