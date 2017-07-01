#ifndef __PALTFORM__
#define __PLATFORM__

#undef PLATFORM_WINDOWS
#define PLATFORM_UNIX

#if defined(PLATFORM_WINDOWS)
	#define LIBRARY_EXPORT __declspec(dllexport)
	#define LIBRARY_IMPORT __declspec(dllimport)
#elif defined(PLATFORM_UNIX)
	#define LIBRARY_EXPORT
	#define LIBRARY_IMPORT
#else
#endif

#endif // __PLATFORM__
