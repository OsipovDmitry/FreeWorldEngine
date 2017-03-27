#include "Core.h"
#include "ThreadManager.h"
#include "Thread.h"

namespace FreeWorldEngine {

ThreadManager::ThreadManager() :
	m_pResourceManager(coreEngine->createResourceManager("ResourceManagerForThreadManager"))
{
}

ThreadManager::~ThreadManager()
{
	coreEngine->destroyResourceManager(m_pResourceManager);
}

IThread *ThreadManager::findThread(const std::string& name) const
{
	return static_cast<IThread*>(m_pResourceManager->findResource(name));
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
	m_pResourceManager->destroyResource(name);
}

void ThreadManager::destroyThread(IThread *pThread)
{
	m_pResourceManager->destroyResource(pThread->name());
}


} // namespace