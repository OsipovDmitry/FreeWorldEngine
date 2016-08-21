#ifndef __GLRENDERERPLUGIN__
#define __GLRENDERERPLUGIN__

#include "IPlugin.h"

namespace FreeWorldEngine {

class IGLRenderer;

class GLRendererPlugin : public IPlugin {
public:
	GLRendererPlugin();
	~GLRendererPlugin();

	std::string name() const;
	std::string info() const;
	bool initialize() const;
	void deinitialize() const;

private:
	static IGLRenderer *pGLRenderer;

}; // class GLRendererPlugin

} // namespace

#endif // __GLRENDERERPLUGIN__