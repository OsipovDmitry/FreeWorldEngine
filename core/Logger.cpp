#include "utility/DateTime.h"

#include "ILog.h"

#include "Logger.h"
#include "TextFileLog.h"
#include "Core.h"

namespace FreeWorldEngine {

Logger::Logger() :
	m_pResourceManager(coreEngine->createResourceManager("ResourceManagerForLogger"))
{
}

Logger::~Logger()
{
	destroyAllLogs();
	coreEngine->destroyResourceManager(m_pResourceManager);
}

ILog *Logger::addTextFileLog(const std::string& filename)
{
	ILog *pLog = findLog(filename);
	if (pLog)
		return pLog;

	pLog = new TextFileLog(filename);
	addLog(pLog);
	return pLog;
}

void Logger::addLog(ILog *pLog)
{
	std::string date = Date::current().toString();
	std::string time = Time::current().toString();
	m_pResourceManager->addResource(pLog);
	pLog->printMessage(ILog::MessageType_Info, time, "Start log on " + date + " at " + time);
}

ILog *Logger::findLog(const std::string& name) const
{
	return reinterpret_cast<ILog*>(m_pResourceManager->findResource(name));
}

void Logger::destroyLog(const std::string& name)
{
	ILog *pLog = findLog(name);
	if (pLog) {
	std::string date = Date::current().toString();
	std::string time = Time::current().toString();
		pLog->printMessage(ILog::MessageType_Info, time, "Finish log on " + date + " at " + time);
		m_pResourceManager->destroyResource(name);
	}
}

void Logger::destroyLog(ILog *pLog)
{
	if (pLog)
		destroyLog(pLog->name());
}

void Logger::destroyAllLogs()
{
	while (m_pResourceManager->size())
		destroyLog(static_cast<ILog*>(*(m_pResourceManager->begin())));
}

void Logger::printMessage(const std::string& message, const ILog::MessageType type) const
{
	std::string time = Time::current().toString();
	for (ResourceIterator iter = m_pResourceManager->begin(); iter != m_pResourceManager->end(); ++iter)
		static_cast<ILog*>(*iter)->printMessage(type, time, message);
}

} // namespace
