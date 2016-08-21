#include "glm/gtc/matrix_transform.hpp"

#include "MovableObject.h"


namespace FreeWorldEngine {

MovableObject::MovableObject()	:
	m_position(0.0f, 0.0f, 0.0f),
	m_orientation(1.0f, 0.0, 0.0f, 0.0f)
{
}

MovableObject::~MovableObject()
{
}

void MovableObject::setPosition(const glm::vec3& pos) {
	m_position = pos;
}

glm::vec3 MovableObject::position() const {
	return m_position;
}

void MovableObject::setOrientation(const glm::quat& orient) {
	m_orientation = orient;
}

glm::quat MovableObject::orientation() const {
	return m_orientation;
}

glm::mat4 MovableObject::modelMatrix() const {
	return glm::translate(glm::mat4_cast(m_orientation), m_position);
}


} // namespace