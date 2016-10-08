#include <algorithm>

#include "FreeWorldEngine.h"
#include "utility/XML.h"

#include "LoggerPlugin.h"
#include "TerminalLog.h"
#include "HtmlLog.h"

namespace FreeWorldEngine {

namespace {
	const std::string g_logerSettingsFileName = "logger.xml";
}

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

	XMLRoot *pXML = XMLRoot::openFromFile(g_logerSettingsFileName);
	if (!pXML) {
		pCore->logger()->printMessage("Could not open file \"" + g_logerSettingsFileName + "\"", ILog::MessageType_Error);
		return false;
	}
	if (pXML->name() != "logs_list") {
		pCore->logger()->printMessage("The document \"" + g_logerSettingsFileName + "\" is damaged", ILog::MessageType_Error);
		XMLRoot::close(pXML);
		return false;
	}

	XMLNode::NodeList nodeList = pXML->children();
	for (XMLNode::NodeList::const_iterator it = nodeList.cbegin(); it != nodeList.cend(); ++it) {
		XMLNode *pNode = *it;
		if (pNode->name() != "log")
			continue;

		const std::string infoColor = pNode->attributeValue("infoColor", "green");
		const std::string warningColor = pNode->attributeValue("warningColor", "yellow");
		const std::string errorColor = pNode->attributeValue("errorColor", "red");
		const std::string criticalColor = pNode->attributeValue("criticalColor", "magenta");

		std::string logType = pNode->attributeValue("type");
		std::transform(logType.begin(), logType.end(), logType.begin(), ::tolower);

		ILog *pLog = 0;
		if (logType == "text") {
			const std::string filename = pNode->attributeValue("file");
			pLog = pCore->logger()->addTextFileLog(filename);
			continue; // текстовый лог не нужно добавлять в логер, так как он уже там есть
		}
		else if (logType == "terminal") {
			pLog = new TerminalLog();
		}
		else if (logType == "html") {
			const std::string filename = pNode->attributeValue("file");
			const std::string backgroundColor = pNode->attributeValue("backgroundColor", "black");
			pLog = new HtmlLog(filename, backgroundColor);
		}
		else {
			pCore->logger()->printMessage("Unresolved type log \"" + logType + "\"", ILog::MessageType_Error);
		}

		if (!pLog)
			continue;

		pLog->setMessageColor(ILog::MessageType_Info, infoColor);
		pLog->setMessageColor(ILog::MessageType_Warning, warningColor);
		pLog->setMessageColor(ILog::MessageType_Error, errorColor);
		pLog->setMessageColor(ILog::MessageType_Critical, criticalColor);
		
		m_logsList.push_back(pLog);
		pCore->logger()->addLog(pLog);
	}

	return true;
}

void LoggerPlugin::deinitialize() const
{
	ICore *pCore = getCoreEngine();
	if (!pCore)
		return;

	for (std::list<ILog*>::iterator it = m_logsList.begin(); it != m_logsList.end(); ++it)
		pCore->logger()->destroyLog(*it);
	m_logsList.clear();
}

} // namespace