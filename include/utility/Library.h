#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>

#include "UtilitySettings.h"

namespace FreeWorldEngine {

namespace Utility {

struct LibraryPrivate;
class UTILITY_EXPORT Library
{
public:
	Library(const std::string& libName = "");
	~Library();

	bool load(const std::string& libName);
	void unload();

	bool isLoaded() const;
	void *resolve(const std::string& funcName) const;

private:
	LibraryPrivate *m;
};

} // namespace

} // namespace

#endif // LIBRARY_H
