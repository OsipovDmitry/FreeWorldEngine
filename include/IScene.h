#ifndef __ISCENE__
#define __ISCENE__

#include "IResource.h"

namespace FreeWorldEngine {

struct Scene;

class IScene : public IResource {
public:
	virtual ~IScene() {}

	virtual const Scene *scene() const = 0;

}; // class IScene

} // namespace

#endif // __ISCENE__