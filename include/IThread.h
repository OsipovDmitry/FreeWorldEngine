#ifndef __ITHREAD__
#define __ITHREAD__

#include "IResource.h"
#include "Types.h"

namespace FreeWorldEngine {

class IThread : public IResource {
public:
	typedef void (*TaskType)(void*);
	
	virtual ~IThread() = 0 {}

	virtual void start() const = 0;
	virtual void finish() const = 0;

	virtual void pushTaskToQueue(TaskType task, void *pParams = 0) = 0;
	virtual uint32 queueSize() const = 0;
	virtual void clearQueue() = 0;
};

} // namespace

#endif // __ITHREAD__