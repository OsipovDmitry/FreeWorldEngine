#ifndef __GLRENDERERSETTINGS__
#define __GLRENDERERSETTINGS__

#ifdef GLRENDERER_EXPORT
#define GLRENDERER_FUNC_DLL __declspec(dllexport) 
#else
#define GLRENDERER_FUNC_DLL __declspec(dllimport) 
#endif

#endif // __GLRENDERERSETTINGS__