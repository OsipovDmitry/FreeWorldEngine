#include <FreeWorldEngine.h>

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

void TextFileLog::printMessage(const ILogger::MessageType type, const std::string& time, const std::string message) const
{
	FILE *file = fopen(m_filename.c_str(), "a");
	if (file) {
		fprintf(file, "%s (%s): %s\n", time.c_str(), ICore::instance()->logger()->messageTypeString(type).c_str(), message.c_str());
		fclose(file);
	}
}

} // namespace
