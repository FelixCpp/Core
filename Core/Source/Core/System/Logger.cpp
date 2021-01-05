#include <Core/System/Logger.hpp>

#include <Windows.h>

static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void ChangeConsoleColorBasedOnLogLevel(const char * logLevel)
{
	WORD wAttributes = 0;

	if (logLevel == "WARN") wAttributes = 14;		// Bright Yellow
	else if (logLevel == "INFO") wAttributes = 10;	// Bright Green
	else if (logLevel == "DEBUG") wAttributes = 11; // Bright Blue
	else if (logLevel == "ERROR") wAttributes = 12; // Bright Red

	/* change the color */
	SetConsoleTextAttribute(hConsole, wAttributes);
}

void ChangeConsoleColorToDefault()
{
	/* change it back to black background with white foreground */
	SetConsoleTextAttribute(hConsole, 7);
}