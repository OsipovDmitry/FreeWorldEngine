#ifndef __THREAD__
#define __THREAD__

#include <queue>
#include <mutex>
#include <thread>
#include "IThread.h"

namespace FreeWorldEngine {

class Thread : public IThread {
public:
	Thread(const std::string& name);
	~Thread();

	std::string name() const;

	void start();
	void finish();

	void pushTaskToQueue(TaskType task, void *pParams = 0);
	uint32 queueSize() const;
	void clearQueue();

private:
	std::queue<std::pair<TaskType, void*> > m_tasks;
	mutable std::mutex m_mutex;
	std::thread *m_pThread;
	bool m_isFinish;
	std::string m_name;

	void mainLoop();
};

} // namespace

#endif // __THREAD__