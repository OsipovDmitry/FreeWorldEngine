#include "FreeWorldEngine.h"

#include "GraphicsEnginePlugin.h"
#include "GraphicsEngine.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsEnginePlugin::GraphicsEnginePlugin()
{
}

GraphicsEnginePlugin::~GraphicsEnginePlugin()
{
}

std::string GraphicsEnginePlugin::name() const
{
	return std::string("GraphicsEnginePlugin");
}

std::string GraphicsEnginePlugin::info() const
{
	return std::string("Graphics Engine Plugin");
}

bool GraphicsEnginePlugin::initialize() const
{
	ICore *pCore = getCoreEngine();
	if (!pCore)
		return false;

	if (!pGraphicsEngine)
		pGraphicsEngine = new GraphicsEngine();

	pCore->setGraphicsEngine(pGraphicsEngine);

	return true;
}

void GraphicsEnginePlugin::deinitialize() const
{
	ICore *pCore = getCoreEngine();

	if (pCore->graphicsEngine() == pGraphicsEngine)
		pCore->setGraphicsEngine(0);

	delete pGraphicsEngine;
	pGraphicsEngine = 0;
}

} // namespace

} // namespace