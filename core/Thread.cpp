#include "Thread.h"

namespace FreeWorldEngine {

Thread::Thread(const std::string& name) :
	m_name(name),
	m_tasks(std::queue<std::pair<TaskType, void*> >()),
	//m_pThread(0),
	m_isFinish(true)
{
}

Thread::~Thread()
{
	finish();
}

std::string Thread::name() const
{
	return m_name;
}

void Thread::start()
{
	//std::lock_guard<std::mutex> locker(m_mutex);
	if (!m_isFinish)
		return;
	m_isFinish = false;
	//m_pThread = new std::thread(&Thread::mainLoop, this);
}

void Thread::finish()
{
	{
		//std::lock_guard<std::mutex> locker(m_mutex);
		if (m_isFinish)
			return;
	}

	clearQueue();

	//m_mutex.lock();
	m_isFinish = true;
	//m_mutex.unlock();

	//m_pThread->join();
	//delete m_pThread;
	//m_pThread = 0;	
}

void Thread::pushTaskToQueue(IThread::TaskType task, void *pParams)
{
	//std::lock_guard<std::mutex> locker(m_mutex);
	m_tasks.push(std::make_pair(task, pParams));
}

uint32 Thread::queueSize() const
{
	//std::lock_guard<std::mutex> locker(m_mutex);
	return m_tasks.size();
}

void Thread::clearQueue()
{
	//std::lock_guard<std::mutex> locker(m_mutex);
	while (m_tasks.size())
		m_tasks.pop();
}

void Thread::mainLoop()
{
	while (true) {
		IThread::TaskType pTask = 0;
		void *pParams = 0;
		{
			//std::lock_guard<std::mutex> locker(m_mutex);
			if (m_isFinish)
				break;
			
			if (!m_tasks.empty()) {
				auto task = m_tasks.front();
				m_tasks.pop();
				pTask = task.first;
				pParams = task.second;
			} else {
				continue;
			}
		}

		if (!pTask)
			continue;
		pTask(pParams);
	}
}

} // namespace