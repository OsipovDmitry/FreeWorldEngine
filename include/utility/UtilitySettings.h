#ifndef __UTILITYSETTINGS__
#define __UTILITYSETTINGS__

#ifdef UTILITY_EXPORT
#define UTILITY_DLL __declspec(dllexport) 
#else
#define UTILITY_DLL __declspec(dllimport) 
#endif

#endif // __UTILITYSETTINGS__