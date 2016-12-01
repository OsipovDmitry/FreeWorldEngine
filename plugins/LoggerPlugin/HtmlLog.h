#ifndef __HTMLLOG__
#define __HTMLLOG__

#include <ILog.h>
#include <ILogger.h>
#include <Types.h>

namespace FreeWorldEngine {

class HtmlLog : public ILog {
public :
	HtmlLog(const std::string& filename, const std::string& backgroundColor = "black");
	~HtmlLog();

	std::string name() const;

	void printMessage(const ILogger::MessageType type, const std::string& time, const std::string message) const;
	void setMessageColor(const ILogger::MessageType type, const std::string& color);

private:
	std::string m_name;
	mutable uint32 m_printPos;
	std::map<ILogger::MessageType, std::string> m_messageColors;
	
	static uint32 printHeader(FILE *pFile, const std::string& backgroundColor);
	static void printFooter(FILE *pFile);

}; // class TerminalLog

} // namespace

#endif // __HTMLLOG__