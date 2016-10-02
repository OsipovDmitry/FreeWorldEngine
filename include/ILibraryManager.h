#ifndef __ILIBRARYMANAGER__
#define __ILIBRARYMANAGER__

#include <string>

namespace FreeWorldEngine {

class ILibrary;

class ILibraryManager {
public:
	virtual ~ILibraryManager() = 0 {}
	virtual ILibrary *loadLibrary(const std::string& filename) = 0;
	virtual void unloadLibrary(const std::string& name) = 0;
	virtual void unloadLibrary(ILibrary *pLibrary) = 0;
	virtual ILibrary *findLibrary(const std::string& name) const = 0;

}; // class ILibraryManager

} // namespace

#endif // __ILIBRARYMANAGER__