#ifndef __CORESETTINGS__
#define __CORESETTINGS__

#include <Platform.h>

#ifdef CORE_EXPORT
#define CORE_EXPORT LIBRARY_EXPORT
#else
#define CORE_EXPORT LIBRARY_IMPORT
#endif

#endif // __CORESETTINGS__
