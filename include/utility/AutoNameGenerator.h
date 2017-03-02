#ifndef __AUTONAMEGENERATOR__
#define __AUTONAMEGENERATOR__

#include <string>

#include "utility/UtilitySettings.h"

namespace FreeWorldEngine {

namespace Utility {

struct AutoNameGeneratorPrivate;
class UTILITY_DLL AutoNameGenerator {
public:
	AutoNameGenerator(const std::string& prefix = "@utoname");
	~AutoNameGenerator();

	std::string generateName();

private:
	AutoNameGeneratorPrivate *m;

}; // class AutoNameGenerator

} // namespace
} // namespace

#endif // __AUTONAMEGENERATOR__