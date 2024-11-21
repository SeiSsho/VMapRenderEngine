#pragma once

#include "Component.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

static const double deg2Rad = M_PI / 180;

class Transform
{
public:
	glm::vec3 _position;
	glm::vec3 _scale;
	glm::quat _rotation;

	glm::vec3 forward() const;
	glm::vec3 up() const;
	glm::vec3 right() const;
	glm::mat4 matrixTransform() const;

	Transform();
	Transform(Transform&&) = default;
	Transform(const Transform&) = default;
	~Transform() = default;
};

