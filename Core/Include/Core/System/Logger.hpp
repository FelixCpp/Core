#pragma once

namespace Core
{
	void ChangeConsoleColorBasedOnLogLevel(const char * logLevel);
	void ChangeConsoleColorToDefault();
}

extern int _cdecl printf(char const * const _Format, ...);

#ifdef CORE_DONT_SHOW_DEBUG
#define CORE_SHOW_DEBUG 0
#else
#define CORE_SHOW_DEBUG 1
#endif // !CORE_DONT_SHOW_DEBUG

#if defined(CORE_SHOW_DEBUG)
#define CORE_LOG(level, format, ...)\
	Core::ChangeConsoleColorBasedOnLogLevel(#level),\
	printf("["#level"]: "##format"\n", __VA_ARGS__),\
	Core::ChangeConsoleColorToDefault()
#else
#define CORE_LOG(level)
#endif

#define CORE_WARN(format, ...) CORE_LOG(WARN, format, __VA_ARGS__)
#define CORE_INFO(format, ...) CORE_LOG(INFO, format, __VA_ARGS__)
#define CORE_DEBUG(format, ...) CORE_LOG(DEBUG, format, __VA_ARGS__)
#define CORE_ERROR(format, ...) CORE_LOG(ERROR, format, __VA_ARGS__)