#pragma once

#include "Transform.h"

class Camera
{
	friend class GUI;
public:
	enum class Type : uint8_t {
		Orthographic = 0x0,
		Perspective = 0x1
	};

	Transform _transform;
	float _fov, _aspect;
	float _left, _right, _bottom, _top, _zoomScale = 1.0f;
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
	void setZoomScale(const float& value);
};

