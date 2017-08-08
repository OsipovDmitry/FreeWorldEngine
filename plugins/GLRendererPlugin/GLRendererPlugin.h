#ifndef __GLRENDERERPLUGIN__
#define __GLRENDERERPLUGIN__

#include <core/IPlugin.h>

namespace FreeWorldEngine {

namespace Renderer {

class GLRendererPlugin : public IPlugin {
public:
	GLRendererPlugin();
	~GLRendererPlugin();

	std::string name() const;
	std::string info() const;
	bool initialize();
	void deinitialize();

}; // class GLRendererPlugin

} // namespace

} // namespace

#endif // __GLRENDERERPLUGIN__
