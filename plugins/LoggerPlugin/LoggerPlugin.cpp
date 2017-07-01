#include <algorithm>

#include <FreeWorldEngine.h>
#include <utility/XML.h>
#include <utility/File.h>

#include "LoggerPlugin.h"
#include "TerminalLog.h"
#include "HtmlLog.h"
#include "TextFileLog.h"

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
	ICore *pCore = ICore::instance();
	if (!pCore)
		return false;

	ILogger *pLogger = pCore->logger();
	if (!pLogger)
		return false;

	Utility::XMLRoot *pXML = Utility::XMLRoot::openFromFile(g_logerSettingsFileName);
	if (!pXML) {
		pLogger->printMessage("Could not open file \"" + g_logerSettingsFileName + "\"", ILogger::MessageType_Error);
		return false;
	}
	if (pXML->name() != "logs_list") {
		pLogger->printMessage("The document \"" + g_logerSettingsFileName + "\" is damaged", ILogger::MessageType_Error);
		Utility::XMLRoot::close(pXML);
		return false;
	}

	Utility::XMLNode::NodeList nodeList = pXML->children();
	for (Utility::XMLNode::NodeList::const_iterator it = nodeList.cbegin(); it != nodeList.cend(); ++it) {
		Utility::XMLNode *pNode = *it;
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
			pLog = new TextFileLog(filename, rewrite);
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
			pLogger->printMessage("Unresolved type log \"" + logType + "\"", ILogger::MessageType_Error);
		}

		if (!pLog)
			continue;

		pLog->setMessageColor(ILogger::MessageType_Info, infoColor);
		pLog->setMessageColor(ILogger::MessageType_Warning, warningColor);
		pLog->setMessageColor(ILogger::MessageType_Error, errorColor);
		pLog->setMessageColor(ILogger::MessageType_Critical, criticalColor);
		pLog->setMessageColor(ILogger::MessageType_Debug, debugColor);

		m_logsList.push_back(pLog);
		pCore->logger()->addLog(pLog);
	}

	Utility::XMLRoot::close(pXML);
	return true;
}

void LoggerPlugin::deinitialize()
{
	ICore *pCore = ICore::instance();
	if (!pCore)
		return;

	ILogger *pLogger = pCore->logger();
	if (!pLogger)
		return;

	for (std::list<ILog*>::iterator it = m_logsList.begin(); it != m_logsList.end(); ++it)
		pLogger->destroyLog(*it);
	m_logsList.clear();
}

} // namespace
