#ifndef __CORESETTINGS__
#define __CORESETTINGS__

#ifdef CORE_EXPORT
#define CORE_FUNC_DLL __declspec(dllexport) 
#else
#define CORE_FUNC_DLL __declspec(dllimport) 
#endif

#endif // __ICORESETTINGS__