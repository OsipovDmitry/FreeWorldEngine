#include "FreeWorldEngine.h"

#include "LoggerPlugin.h"
#include "TerminalLog.h"

namespace FreeWorldEngine {

LoggerPlugin::LoggerPlugin() :
	m_logsList()
{
}

LoggerPlugin::~LoggerPlugin()
{
}

std::string LoggerPlugin::name() const
{
	return std::string("LoggerPlugin");
}

std::string LoggerPlugin::info() const
{
	return std::string("Logger Plugin");
}

bool LoggerPlugin::initialize() const
{
	ICore *pCore = getCoreEngine();
	if (!pCore)
		return false;

	m_logsList.push_back(new TerminalLog());
	pCore->logger()->addLog(m_logsList.back());

	return true;
}

void LoggerPlugin::deinitialize() const
{
	ICore *pCore = getCoreEngine();
	if (!pCore)
		return;

	for (std::list<ILog*>::iterator it = m_logsList.begin(); it != m_logsList.end(); ++it)
		pCore->logger()->deleteLog(*it);
	m_logsList.clear();
}

} // namespace