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
	std::string date = coreEngine->currentDate().toString();
	std::string time = coreEngine->currentTime().toString();
	m_pResourceManager->addResource(pLog);
	pLog->printMessage(ILog::MessageType_Info, time, "Start log on " + date + " at " + time);
}

ILog *Logger::getLogByName(const std::string& name) const
{
	return reinterpret_cast<ILog*>(m_pResourceManager->getByName(name));
}

void Logger::deleteLog(const std::string& name)
{
	ILog *pLog = getLogByName(name);
	if (pLog) {
		std::string date = coreEngine->currentDate().toString();
		std::string time = coreEngine->currentTime().toString();
		pLog->printMessage(ILog::MessageType_Info, coreEngine->currentTime().toString(), "Finish log on " + date + " at " + time);
		m_pResourceManager->deleteResource(name);
	}
}

void Logger::deleteLog(ILog *pLog)
{
	if (pLog)
		deleteLog(pLog->name());
}

void Logger::printMessage(const std::string& message, const ILog::MessageType type) const
{
	std::string time = coreEngine->currentTime().toString();
	for (ResourceIterator iter = m_pResourceManager->begin(); iter != m_pResourceManager->end(); ++iter)
		static_cast<ILog*>(*iter)->printMessage(type, time, message);
}

} // namespace
