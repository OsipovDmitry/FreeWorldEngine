#ifndef __GRAPHICSENGINEPLUGIN__
#define __GRAPHICSENGINEPLUGIN__

#include "IPlugin.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

class GraphicsEnginePlugin : public IPlugin {
public:
	GraphicsEnginePlugin();
	~GraphicsEnginePlugin();

	std::string name() const;
	std::string info() const;
	bool initialize() const;
	void deinitialize() const;

}; // class GraphicsEnginePlugin

} // namespace

} // namespace

#endif // __GRAPHICSENGINEPLUGIN__