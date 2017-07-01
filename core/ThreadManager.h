#ifndef __THREADMANAGER__
#define __THREADMANAGER__

#include <core/IThreadManager.h>

namespace FreeWorldEngine {

class IResourceManager;

class ThreadManager : public IThreadManager {
public:
	ThreadManager();
	~ThreadManager();

	IThread *findThread(const std::string& name) const;
	IThread *createThread(const std::string& name);
	void destroyThread(const std::string& name);
	void destroyThread(IThread *pThread);

private:
	IResourceManager *m_pResourceManager;

}; // class ThreadManager

} // namespace

#endif // __THREADMANAGER__
