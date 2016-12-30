#ifndef __GLRENDERERPLUGIN__
#define __GLRENDERERPLUGIN__

#include "IPlugin.h"

namespace FreeWorldEngine {

namespace Renderer {

class GLRendererPlugin : public IPlugin {
public:
	GLRendererPlugin();
	~GLRendererPlugin();

	std::string name() const;
	std::string info() const;
	bool initialize() const;
	void deinitialize() const;

}; // class GLRendererPlugin

} // namespace

} // namespace

#endif // __GLRENDERERPLUGIN__