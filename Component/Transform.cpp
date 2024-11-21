#include "Transform.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>

glm::vec3 Transform::forward() const {
	return _rotation * glm::vec3(0.0, 0.0, 1.0);
}

glm::vec3 Transform::up() const {
	return _rotation * glm::vec3(0.0, 1.0, 0.0);
}

glm::vec3 Transform::right() const {
	return _rotation * glm::vec3(1.0, 0.0, 0.0);
}

glm::mat4 Transform::matrixTransform() const {
	glm::mat4 positionMtx = glm::translate(glm::mat4(1.0), _position);
	glm::mat4 scaleMtx = glm::scale(glm::mat4(1.0), _scale);
	glm::mat4 rotationMtx = glm::mat4_cast(_rotation);
	return positionMtx * rotationMtx * scaleMtx;
}

Transform::Transform()
	:	_position(glm::vec3(0.0f)),
		_scale(glm::vec3(1.0)),
		_rotation(glm::quat(1.0f, 0.0, 0.0, 0.0))
{
}
