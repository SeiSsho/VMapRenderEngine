#include "Camera.h"

Camera::Camera(const Transform& transform, const float& fov, const float& aspect, const float& near, const float& far)
	: _transform(transform), _fov(fov), _aspect(aspect), _near(near), _far(far), _cameraType(Type::Perspective)
{ }

Camera::Camera(const Transform& transform, 
	const float& left, const float& right, const float& top, const float& bottom,
	const float& near, const float& far)
	: _transform(transform),
		_left(left), _right(right), _top(top), _bottom(bottom),
		_near(near), _far(far), _cameraType(Type::Orthographic)
{ }

void Camera::setPerspectiveProjection(const float& fov, const float& aspect, const float& near, const float& far) {
	_fov = fov;
	_aspect = aspect;
	_near = near;
	_far = far;
	_cameraType = Type::Perspective;
}

void Camera::setOrthographicProjection(
	const float& left, const float& right, const float& top, const float& bottom,
	const float& near, const float& far) {
	_left = left;
	_right = right;
	_top = top;
	_bottom = bottom;
	_near = near;
	_far = far;
	_cameraType = Type::Orthographic;
}

glm::mat4 Camera::getViewMatrix() const {
	return glm::lookAt(_transform._position, _transform._position + _transform.forward(), _transform.up());
}

glm::mat4 Camera::getProjectionMatrix() const {
	switch (_cameraType) {
	case Type::Perspective:
		return glm::perspective(glm::radians(_fov), 16.0f / 9.0f, _near, _far);
	case Type::Orthographic:
		return glm::ortho(_left * _zoomScale, _right * _zoomScale, _bottom * _zoomScale, _top * _zoomScale, _near, _far);
	}
}

void Camera::setZoomScale(const float& value) {
	_zoomScale = value;
}
