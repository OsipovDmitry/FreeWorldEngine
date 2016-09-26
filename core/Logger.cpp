#include "ILog.h"

#include "Logger.h"
#include "Core.h"

namespace FreeWorldEngine {

Logger::Logger() :
	m_pResourceManager(coreEngine->createResourceManager("ResourceManagerForLogger"))
{
}

Logger::~Logger()
{
	coreEngine->destroyResourceManager(m_pResourceManager);
}

void Logger::addLog(ILog *pLog)
{
	m_pResourceManager->addResource(pLog);
}

ILog *Logger::getLogByName(const std::string& name) const
{
	return reinterpret_cast<ILog*>(m_pResourceManager->getByName(name));
}

void Logger::deleteLog(const std::string& name)
{
	m_pResourceManager->deleteResource(name);
}

void Logger::deleteLog(ILog *pLog)
{
	if (pLog)
		deleteLog(pLog->name());
}

void Logger::printMessage(const std::string& message, const ILog::MessageType type) const
{
	for (ResourceIterator iter = m_pResourceManager->begin(); iter != m_pResourceManager->end(); ++iter)
		static_cast<ILog*>(*iter)->printMessage(type, "", message);
}

} // namespace
