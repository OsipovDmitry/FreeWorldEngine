#include "HtmlLog.h"

namespace FreeWorldEngine {

namespace {
	const std::string footerStr = "</BODY>\n"
								  "</HTML>\n";

}

HtmlLog::HtmlLog(const std::string& filename, const std::string& backgroundColor) :
	m_name(filename),
	m_printPos(0),
	m_messageColors(std::map<ILog::MessageType, std::string>())
{
	FILE *pFile = fopen(m_name.c_str(), "w");
	if (pFile) {
		m_printPos = printHeader(pFile, backgroundColor);
		printFooter(pFile);
		fclose(pFile);
	}

	setMessageColor(ILog::MessageType_Info, "green");
	setMessageColor(ILog::MessageType_Warning, "yellow");
	setMessageColor(ILog::MessageType_Error, "red");
	setMessageColor(ILog::MessageType_Critical, "magenta");
}

HtmlLog::~HtmlLog()
{
}

std::string HtmlLog::name() const
{
	return m_name;
}

void HtmlLog::printMessage(const MessageType type, const std::string& time, const std::string message) const
{
	std::string typeStr = messageType(type);
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

void HtmlLog::setMessageColor(const ILog::MessageType type, const std::string& color)
{
	m_messageColors[type] = color;
}

std::string HtmlLog::messageType(const MessageType type)
{
	switch (type) {
	case MessageType_Info: return "INFO";
	case MessageType_Warning: return "WARNING";
	case MessageType_Error: return "ERROR";
	case MessageType_Critical: return "CRITICAL";
	default: return "None";
	}
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