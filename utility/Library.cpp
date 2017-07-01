#include <utility/Library.h>

#if defined(PLATFORM_WINDOWS)
#include <Windows.h>
#elif defined(PLATFORM_UNIX)
#include <dlfcn.h>
#else
#endif

namespace FreeWorldEngine {

namespace Utility {

struct LibraryPrivate {
#if defined(PLATFORM_WINDOWS)
HINSTANCE instance;
#elif defined(PLATFORM_UNIX)
void *instance;
#else
#endif
};

Library::Library(const std::string& libName) :
	m(new LibraryPrivate)
{
	load(libName);
}

Library::~Library()
{
	unload();
	delete m;
}

bool Library::load(const std::string& libName)
{
	unload();
#if defined(PLATFORM_WINDOWS)
	wchar_t *wname = new wchar_t[libName.size()+8];
	mbstowcs(wname, libName.c_str(), libName.size()+1);
	m->instance = LoadLibrary((LPCWSTR)wname);
	delete [] wname;
#elif defined(PLATFORM_UNIX)
	m->instance = dlopen(libName.c_str(), RTLD_LAZY);
#else
	;
#endif
	return isLoaded();
}

void Library::unload()
{
	if (!isLoaded())
		return;
#if defined(PLATFORM_WINDOWS)
	FreeLibrary(m->instance);
	m->instance = 0;
#elif defined(PLATFORM_UNIX)
	dlclose(m->instance);
	m->instance = nullptr;
#else
	;
#endif
}

bool Library::isLoaded() const
{
#if defined(PLATFORM_WINDOWS)
	return m->instance != 0;
#elif defined(PLATFORM_UNIX)
	return m->instance != nullptr;
#else
	return false;
#endif
}

void* Library::resolve(const std::string& funcName) const
{
	if (!isLoaded())
		return nullptr;
#if defined(PLATFORM_WINDOWS)
	return GetProcAddress(m->instance, funcName.c_str());
#elif defined(PLATFORM_UNIX)
	return dlsym(m->instance, funcName.c_str());
#else
	return false;
#endif
}

} // namespace

} // namespace
