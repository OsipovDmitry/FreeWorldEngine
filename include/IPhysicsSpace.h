#ifndef __IPHYSICSSPACE__
#define __IPHYSICSSPACE__

#include "IPhysicsGeometry.h"

namespace FreeWorldEngine {

class IPhysicsSpace : public IPhysicsGeometry {
public:
	virtual void insertPhysicsGeometry(IPhysicsGeometry *pGeometry) = 0; // Поместить геометрию (или пространство) в пространство.
	virtual void removePhysicsGeometry(IPhysicsGeometry *pGeometry) = 0; // Удалить геометрию из пространства.
	virtual bool queryPhysicsGeometry(IPhysicsGeometry *pGeometry) const = 0; // Проверха нахождения геометрии в пространстве. 
	virtual void setCleanup(const bool state) = 0; // Установка режима уничтожения геомтрии при уничтожении пространства. По умолчанию true.
	virtual bool isCleanup() const = 0; // Режим очистки.

}; // class IPhysicsSpace

} // namespace

#endif // __IPHYSICSSPACE__