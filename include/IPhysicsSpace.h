#ifndef __IPHYSICSSPACE__
#define __IPHYSICSSPACE__

#include "IPhysicsGeometry.h"

namespace FreeWorldEngine {

class IPhysicsSpace : public IPhysicsGeometry {
public:
	virtual void insertPhysicsGeometry(IPhysicsGeometry *pGeometry) = 0; // ��������� ��������� (��� ������������) � ������������.
	virtual void removePhysicsGeometry(IPhysicsGeometry *pGeometry) = 0; // ������� ��������� �� ������������.
	virtual bool queryPhysicsGeometry(IPhysicsGeometry *pGeometry) const = 0; // �������� ���������� ��������� � ������������. 
	virtual void setCleanup(const bool state) = 0; // ��������� ������ ����������� �������� ��� ����������� ������������. �� ��������� true.
	virtual bool isCleanup() const = 0; // ����� �������.

}; // class IPhysicsSpace

} // namespace

#endif // __IPHYSICSSPACE__