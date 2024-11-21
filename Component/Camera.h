#pragma once

#include "Transform.h"

class Camera
{
public:
	enum Type {
		Orthographic,
		Perspective
	};

	Transform _transform;
	float _fov, _aspect;
	float _left, _right, _bottom, _top;
	float _near, _far;
	Type _cameraType;

	Camera(const Transform& transform, 
		const float& fov, 
		const float& aspect, 
		const float& near, const float& far);

	Camera(const Transform& transform,
		const float& left, const float& right,
		const float& top, const float& bottom,
		const float& near, const float& far);

	void setPerspectiveProjection(const float& fov, const float& aspect, const float& near, const float& far);
	void setOrthographicProjection(const float& left, const float& right, const float& top, const float& bottom, 
		const float& near, const float& far);
	

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;
};

