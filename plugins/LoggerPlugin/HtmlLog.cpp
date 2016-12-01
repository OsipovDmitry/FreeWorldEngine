#include <FreeWorldEngine.h>

#include "HtmlLog.h"

namespace FreeWorldEngine {

namespace {
	const std::string footerStr = "</BODY>\n"
								  "</HTML>\n";

}

HtmlLog::HtmlLog(const std::string& filename, const std::string& backgroundColor) :
	m_name(filename),
	m_printPos(0),
	m_messageColors(std::map<ILogger::MessageType, std::string>())
{
	FILE *pFile = fopen(m_name.c_str(), "w");
	if (pFile) {
		m_printPos = printHeader(pFile, backgroundColor);
		printFooter(pFile);
		fclose(pFile);
	}

	setMessageColor(ILogger::MessageType_Info, "green");
	setMessageColor(ILogger::MessageType_Warning, "yellow");
	setMessageColor(ILogger::MessageType_Error, "red");
	setMessageColor(ILogger::MessageType_Critical, "magenta");
	setMessageColor(ILogger::MessageType_Debug, "blue");
}

HtmlLog::~HtmlLog()
{
}

std::string HtmlLog::name() const
{
	return m_name;
}

void HtmlLog::printMessage(const ILogger::MessageType type, const std::string& time, const std::string message) const
{
	std::string typeStr = getCoreEngine()->logger()->messageTypeString(type);
	std::string color = m_messageColors.at(type);
	std::string messageStr = "<p><font color=\"" + color + "\">" + time + " (" + typeStr + "): " + message + "</font></p>";

	FILE *pFile = fopen(m_name.c_str(), "a");
	if (pFile) {
		fseek(pFile, m_printPos, SEEK_SET);
		fprintf(pFile, messageStr.c_str());
		m_printPos += messageStr.length();
		printFooter(pFile);
		fclose(pFile);
	}
}

void HtmlLog::setMessageColor(const ILogger::MessageType type, const std::string& color)
{
	m_messageColors[type] = color;
}

uint32 HtmlLog::printHeader(FILE *pFile, const std::string& backgroundColor)
{
	const std::string headerStr = "<HTML>\n"
								  "<HEAD>\n"
								  "<TITLE>Log File</TITLE>\n"
								  "</HEAD>\n"
								  "<BODY bgcolor=\"" + backgroundColor + "\">\n";
	fprintf(pFile, headerStr.c_str());
	return headerStr.length();
}

void HtmlLog::printFooter(FILE *pFile)
{
	fprintf(pFile, footerStr.c_str());
}

} // namespace