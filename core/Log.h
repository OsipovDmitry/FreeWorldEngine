#ifndef __LOG__
#define __LOG__

#include <ILog.h>

namespace FreeWorldEngine {

class Log : public ILog {
public:
	Log(const std::string& filename);
	~Log();
	void add(const std::string& text) const;
	const ILog& operator <<(const std::string& text) const;
private:
	std::string m_filename;

}; // class Log

} // namespace

#endif // __LOG__