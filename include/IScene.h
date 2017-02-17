#ifndef __ISCENE__
#define __ISCENE__

#include "IResource.h"

namespace FreeWorldEngine {

struct SceneData;

class IScene : public IResource {
public:
	virtual ~IScene() {}

	virtual const SceneData *scene() const = 0;

}; // class IScene

} // namespace

#endif // __ISCENE__