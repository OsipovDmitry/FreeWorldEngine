#ifndef __TEXTFILELOG__
#define __TEXTFILELOG__

#include <ILog.h>

namespace FreeWorldEngine {

class TextFileLog : public ILog {
public :
	TextFileLog(const std::string filename, const bool rewrite = false);
	~TextFileLog();

	std::string name() const;

	void printMessage(const MessageType type, const std::string& time, const std::string message) const;

private:
	std::string m_filename;

	static std::string messageType(const MessageType type);

}; // class TextFileLog

} // namespace

#endif // __TEXTFILELOG__