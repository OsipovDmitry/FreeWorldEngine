#ifndef __MATHSETTINGS__
#define __MATHSETTINGS__

#include <Platform.h>

#ifdef MATH_EXPORT
#define MATH_EXPORT LIBRARY_EXPORT
#else
#define MATH_EXPORT LIBRARY_IMPORT
#endif

#endif // __MATHSETTINGS__
