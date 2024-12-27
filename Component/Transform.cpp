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

glm::vec3 Transform::getEulerAngle() const {
    glm::vec3 angles;

    // Roll (x-axis rotation)
    float sinr_cosp = 2 * (_rotation.w * _rotation.x + _rotation.y * _rotation.z);
    float cosr_cosp = 1 - 2 * (_rotation.x * _rotation.x + _rotation.y * _rotation.y);
    angles.x = glm::degrees(std::atan2(sinr_cosp, cosr_cosp));

    // Pitch (y-axis rotation)
    float sinp = 2 * (_rotation.w * _rotation.y - _rotation.z * _rotation.x);
    if (std::abs(sinp) >= 1)
        angles.y = glm::degrees(std::copysign(glm::pi<float>() / 2, sinp)); // use 90 degrees if out of range
    else
        angles.y = glm::degrees(std::asin(sinp));

    // Yaw (z-axis rotation)
    float siny_cosp = 2 * (_rotation.w * _rotation.z + _rotation.x * _rotation.y);
    float cosy_cosp = 1 - 2 * (_rotation.y * _rotation.y + _rotation.z * _rotation.z);
    angles.z = glm::degrees(std::atan2(siny_cosp, cosy_cosp));
    return angles;
}
