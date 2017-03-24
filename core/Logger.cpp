#include <algorithm>

#include "utility/DateTime.h"
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
	destroyAllLogs();
	coreEngine->destroyResourceManager(m_pResourceManager);
}


void Logger::addLog(ILog *pLog)
{
	std::string date = Utility::Date::current().toString();
	std::string time = Utility::Time::current().toString();
	m_pResourceManager->addResource(pLog);
	pLog->printMessage(MessageType_Info, time, "Start log on " + date + " at " + time);
}

ILog *Logger::findLog(const std::string& name) const
{
	return reinterpret_cast<ILog*>(m_pResourceManager->findResource(name));
}

void Logger::destroyLog(const std::string& name)
{
	ILog *pLog = findLog(name);
	destroyLog(pLog);
}

void Logger::destroyLog(ILog *pLog)
{
	if (pLog) {
		std::string date = Utility::Date::current().toString();
		std::string time = Utility::Time::current().toString();
		pLog->printMessage(MessageType_Info, time, "Finish log on " + date + " at " + time);
		m_pResourceManager->destroyResource(pLog->name());
	}
}

void Logger::destroyAllLogs()
{
	while (m_pResourceManager->size())
		destroyLog(static_cast<ILog*>(*(m_pResourceManager->begin())));
}

void Logger::printMessage(const std::string& message, const MessageType type) const
{
#ifndef _DEBUG // только release
	if (type == MessageType_Debug)
		return;
#endif

	std::string time = Utility::Time::current().toString();

	std::for_each(m_pResourceManager->begin(), m_pResourceManager->end(), [&type, &time, &message](IResource* p) {
		static_cast<ILog*>(p)->printMessage(type, time, message);
	});
}

std::string Logger::messageTypeString(const MessageType type) const
{
	switch (type) {
	case MessageType_Info: return "INFO";
	case MessageType_Warning: return "WARNING";
	case MessageType_Error: return "ERROR";
	case MessageType_Critical: return "CRITICAL";
	case MessageType_Debug: return "DEBUG";
	}
	return "None";
}

} // namespace
