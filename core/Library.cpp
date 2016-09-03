#include "Library.h"

namespace FreeWorldEngine {

Library::Library(const std::string& libraryName) :
	m_name(libraryName),
#ifdef WIN32
	hInstance(0)
#endif
{
	load();
}

Library::~Library()
{
	unload();
}

std::string Library::name() const
{
	return m_name;
}

bool Library::isLoaded() const
{
#ifdef WIN32
	return hInstance != 0;
#endif
}

void *Library::resolve(const std::string& funcName) const
{
	void *pFunc = 0;

#ifdef WIN32
	if (hInstance)
		pFunc = GetProcAddress(hInstance, funcName.c_str());
#endif

	return pFunc;
}

void Library::load()
{
#ifdef WIN32
	wchar_t *wname = new wchar_t[m_name.size()+8];
	mbstowcs(wname, m_name.c_str(), m_name.size()+1);
	hInstance = LoadLibrary((LPCWSTR)wname);
	delete [] wname;
#endif
}

void Library::unload()
{
#ifdef WIN32
	FreeLibrary(hInstance);
	hInstance = 0;
#endif
}

} // namespace