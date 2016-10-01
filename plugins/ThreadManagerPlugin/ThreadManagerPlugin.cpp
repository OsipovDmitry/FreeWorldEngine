#include "FreeWorldEngine.h"

#include "ThreadManagerPlugin.h"
#include "ThreadManager.h"

namespace FreeWorldEngine {

IThreadManager *ThreadManagerPlugin::pThreadManager = 0;

ThreadManagerPlugin::ThreadManagerPlugin()
{
}

ThreadManagerPlugin::~ThreadManagerPlugin()
{
}

std::string ThreadManagerPlugin::name() const
{
	return std::string("ThreadManagerPlugin");
}

std::string ThreadManagerPlugin::info() const
{
	return std::string("std::thread Thread Manager Plugin");
}

bool ThreadManagerPlugin::initialize() const
{
	ICore *pCore = getCoreEngine();
	if (!pCore)
		return false;

	if (!pThreadManager)
		pThreadManager = new ThreadManager();

	pCore->setThreadManager(pThreadManager);

	return true;
}

void ThreadManagerPlugin::deinitialize() const
{
	ICore *pCore = getCoreEngine();

	if (pCore->threadManager() == pThreadManager)
		pCore->setThreadManager(0);

	delete pThreadManager;
	pThreadManager = 0;
}

} // namespace