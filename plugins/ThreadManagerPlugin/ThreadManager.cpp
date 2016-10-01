#include "FreeWorldEngine.h"

#include "ThreadManager.h"
#include "Thread.h"

namespace FreeWorldEngine {

ThreadManager::ThreadManager() :
	m_pResourceManager(getCoreEngine()->createResourceManager("ResourceManagerForThreadManager"))
{
}

ThreadManager::~ThreadManager()
{
	getCoreEngine()->destroyResourceManager(m_pResourceManager);
}

IThread *ThreadManager::findThread(const std::string& name) const
{
	return static_cast<IThread*>(m_pResourceManager->getByName(name));
}

IThread *ThreadManager::createThread(const std::string& name)
{
	IThread *pThread = findThread(name);
	if (pThread)
		return pThread;

	pThread = new Thread(name);
	m_pResourceManager->addResource(pThread);
	return pThread;
}

void ThreadManager::destroyThread(const std::string& name)
{
	m_pResourceManager->deleteResource(name);
}

void ThreadManager::destroyThread(IThread *pThread)
{
	m_pResourceManager->deleteResource(pThread);
}


} // namespace