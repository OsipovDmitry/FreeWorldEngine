#include "TextFileLog.h"

namespace FreeWorldEngine {

TextFileLog::TextFileLog(const std::string filename, const bool rewrite) :
	m_filename(filename)
{
	if (rewrite) {
		FILE *file = fopen(m_filename.c_str(), "w");
		fclose(file);
	}
}

TextFileLog::~TextFileLog()
{
}

std::string TextFileLog::name() const
{
	return m_filename;
}

void TextFileLog::printMessage(const MessageType type, const std::string& time, const std::string message) const
{
	FILE *file = fopen(m_filename.c_str(), "a");
	if (file) {
		fprintf(file, "%s (%s): %s\n", time.c_str(), messageType(type).c_str(), message.c_str());
		fclose(file);
	}
}

std::string TextFileLog::messageType(const MessageType type)
{
	switch (type) {
	case MessageType_Info: return "INFO";
	case MessageType_Warning: return "WARNING";
	case MessageType_Error: return "ERROR";
	case MessageType_Critical: return "CRITICAL";
	default: return "None";
	}
}

} // namespace