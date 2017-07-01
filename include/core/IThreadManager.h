#ifndef __ITHREADMANAGER__
#define __ITHREADMANAGER__

#include <string>

namespace FreeWorldEngine {

class IThread;

class IThreadManager {
public:
	virtual ~IThreadManager() {}

	virtual IThread *findThread(const std::string& name) const = 0;
	virtual IThread *createThread(const std::string& name) = 0;
	virtual void destroyThread(const std::string& name) = 0;
	virtual void destroyThread(IThread *pThread) = 0;

}; // class IThreadManager

} // namespace

#endif // __ITHREADMANAGER__
