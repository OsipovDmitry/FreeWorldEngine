#ifndef __ISCENELOADER__
#define __ISCENELOADER__

#include <string>

namespace FreeWorldEngine {

class IScene;

class ISceneLoader {
public:
	virtual ~ISceneLoader() {}
	virtual IScene *loadScene(const std::string& filename) = 0;
	virtual IScene *findScene(const std::string& name) const = 0;
	virtual void destoryScene(const std::string& name) = 0;
	virtual void destoryScene(IScene *pImage) = 0;

}; // class ISceneLoader

} // namespace

#endif // __ISCENELOADER__