#ifndef __KEYGENERATOR__
#define __KEYGENERATOR__

#include "utility/UtilitySettings.h"
#include "Types.h"

namespace FreeWorldEngine {

namespace Utility {

struct KeyGeneratorPrivate;
class UTILITY_EXPORT KeyGenerator {
public:
	KeyGenerator(const uint32 startId = 1);
	~KeyGenerator();

	uint32 generateId();
	void clearId(const uint32 id);

private:
	KeyGeneratorPrivate *m;

}; // class KeyGenerator

} // namespace
} // namespace

#endif // __KEYGENERATOR__
