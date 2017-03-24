#ifndef __TEXTFILELOG__
#define __TEXTFILELOG__

#include <ILog.h>

namespace FreeWorldEngine {

class TextFileLog : public ILog {
public :
	TextFileLog(const std::string filename, const bool rewrite);
	~TextFileLog();

	std::string name() const;

	void printMessage(const ILogger::MessageType type, const std::string& time, const std::string message) const;
	void setMessageColor(const ILogger::MessageType type, const std::string& color) {}

private:
	std::string m_filename;

}; // class TextFileLog

} // namespace

#endif // __TEXTFILELOG__