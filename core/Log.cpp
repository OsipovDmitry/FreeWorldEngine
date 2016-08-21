#include "Log.h"

namespace FreeWorldEngine {

Log::Log(const std::string& filename) :
	m_filename(filename)
{
}

Log::~Log()
{
}

void Log::add(const std::string& text) const
{
	FILE *file = fopen(m_filename.c_str(), "a");
	if (file) {
		fprintf(file, "%s\n", text.c_str());
		fclose(file);
	}
}

const ILog& Log::operator <<(const std::string& text) const
{
	add(text);
	return (const ILog&)*this;
}

} // namespace