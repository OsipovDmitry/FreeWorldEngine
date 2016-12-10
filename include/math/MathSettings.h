#ifndef __MATHSETTINGS__
#define __MATHSETTINGS__

#ifdef MATH_EXPORT
#define MATH_DLL __declspec(dllexport) 
#else
#define MATH_DLL __declspec(dllimport) 
#endif

#endif // __MATHSETTINGS__