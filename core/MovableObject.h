#ifndef __MOVABLEOBJECT__
#define __MOVABLEOBJECT__

#include "IMovableObject.h"

namespace FreeWorldEngine {
	
class MovableObject : public IMovableObject {
public:
	MovableObject();
	~MovableObject();
	void setPosition(const glm::vec3& pos);
	glm::vec3 position() const;
	void setOrientation(const glm::quat& orient);
	glm::quat orientation() const; 
	glm::mat4 modelMatrix() const;

private:
	glm::vec3 m_position;
	glm::quat m_orientation;

}; // class MovableObject

} // namespace

#endif // __MOVABLEOBJECT__