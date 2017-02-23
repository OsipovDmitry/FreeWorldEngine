#include <algorithm>

#include <FreeWorldEngine.h>
#include <utility/XML.h>

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

bool LoggerPlugin::initialize()
{
	ICore *pCore = getCoreEngine();
	if (!pCore)
		return false;

	XMLRoot *pXML = XMLRoot::openFromFile(g_logerSettingsFileName);
	if (!pXML) {
		pCore->logger()->printMessage("Could not open file \"" + g_logerSettingsFileName + "\"", ILogger::MessageType_Error);
		return false;
	}
	if (pXML->name() != "logs_list") {
		pCore->logger()->printMessage("The document \"" + g_logerSettingsFileName + "\" is damaged", ILogger::MessageType_Error);
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
		const std::string debugColor = pNode->attributeValue("debugColor", "blue");

		std::string logType = pNode->attributeValue("type");
		std::transform(logType.begin(), logType.end(), logType.begin(), ::tolower);

		ILog *pLog = 0;
		if (logType == "text") {
			const std::string filename = pNode->attributeValue("file");
			const bool rewrite = pNode->attributeValue("rewrite", "true") != "false";
			pLog = pCore->logger()->addTextFileLog(filename, rewrite);
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
			pCore->logger()->printMessage("Unresolved type log \"" + logType + "\"", ILogger::MessageType_Error);
		}

		if (!pLog)
			continue;

		pLog->setMessageColor(ILogger::MessageType_Info, infoColor);
		pLog->setMessageColor(ILogger::MessageType_Warning, warningColor);
		pLog->setMessageColor(ILogger::MessageType_Error, errorColor);
		pLog->setMessageColor(ILogger::MessageType_Critical, criticalColor);
		pLog->setMessageColor(ILogger::MessageType_Debug, debugColor);

		if (logType != "text") { // текстовый лог не нужно добавлять в логер, так как он добавится туда в инструкции pCore->logger()->addTextFileLog(filename)
			m_logsList.push_back(pLog);
			pCore->logger()->addLog(pLog);
		}
	}

	return true;
}

void LoggerPlugin::deinitialize()
{
	ICore *pCore = getCoreEngine();
	if (!pCore)
		return;

	for (std::list<ILog*>::iterator it = m_logsList.begin(); it != m_logsList.end(); ++it)
		pCore->logger()->destroyLog(*it);
	m_logsList.clear();
}

} // namespace