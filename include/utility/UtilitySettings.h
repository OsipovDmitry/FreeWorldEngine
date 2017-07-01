#ifndef __UTILITYSETTINGS__
#define __UTILITYSETTINGS__

#include <Platform.h>

#ifdef UTILITY_EXPORT
#define UTILITY_EXPORT LIBRARY_EXPORT
#else
#define UTILITY_EXPORT LIBRARY_IMPORT
#endif

#endif // __UTILITYSETTINGS__
