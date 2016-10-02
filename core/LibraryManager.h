#ifndef __LIBRARYMANAGER__
#define __LIBRARYMANAGER__

#include "ILibraryManager.h"

namespace FreeWorldEngine {

class IResourceManager;

class LibraryManager : public ILibraryManager {
public:
	LibraryManager();
	~LibraryManager();
	
	ILibrary *loadLibrary(const std::string& filename);
	void unloadLibrary(const std::string& name);
	void unloadLibrary(ILibrary *pLibrary);
	ILibrary *findLibrary(const std::string& name) const;

private:
	IResourceManager *m_pResourceManager;

}; // class LibraryManager

} // namespace

#endif // __LIBRARYMANAGER__