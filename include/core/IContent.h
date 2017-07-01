#ifndef __ICONTENT__
#define __ICONTENT__

#include "IResource.h"

namespace FreeWorldEngine {

template <class DataType>
class IContent : public IResource {
public:
	virtual ~IContent() {}

	virtual const DataType *data() const = 0;
};

} // namespace

#endif //__ICONTENT__