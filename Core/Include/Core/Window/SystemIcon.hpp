#pragma once

namespace Core
{
	enum class SystemIcon {
		Application = 0x7F00, // Default application icon.
		Error		= 0x7F01, // Hand-shaped icon.
		Question	= 0x7F02, // Question mark icon.
		Exclamation = 0x7F03, // Exclamation point icon.
		Asterisk	= 0x7F04, // Asterisk icon.
		Winlogo		= 0x7F05, // Windows 2000: Windows logo icon
		Shield		= 0x7F06, // Security Shield icon.

		Information = Asterisk,
		Warning = Exclamation,
		Hand = Error
	};

}