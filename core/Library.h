#ifndef __LIBRARY__
#define __LIBRARY__

#ifdef WIN32
	#include <Windows.h>
#endif

#include "ILibrary.h" 

namespace FreeWorldEngine {

class Library : public ILibrary {
public:
	Library(const std::string& libraryName);
	~Library();

	std::string name() const;
	bool isLoaded() const;
	void *resolve(const std::string& funcName) const;

private:
	std::string m_name;

#ifdef WIN32
	HINSTANCE hInstance;
#endif

	void load();
	void unload();

}; // class Library

} // namespace

#endif // __LIBRARY__