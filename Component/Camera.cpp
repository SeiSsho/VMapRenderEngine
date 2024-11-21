#include "Camera.h"

Camera::Camera(const Transform& transform, const float& fov, const float& aspect, const float& near, const float& far)
	: _transform(transform), _fov(fov), _aspect(aspect), _near(near), _far(far), _cameraType(Perspective)
{ }

Camera::Camera(const Transform& transform, 
	const float& left, const float& right, const float& top, const float& bottom,
	const float& near, const float& far)
	: _transform(transform),
		_left(left), _right(right), _top(top), _bottom(bottom),
		_near(near), _far(far), _cameraType(Camera::Orthographic)
{ }

void Camera::setPerspectiveProjection(const float& fov, const float& aspect, const float& near, const float& far) {
	_fov = fov;
	_aspect = aspect;
	_near = near;
	_far = far;
	_cameraType = Perspective;
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
	_cameraType = Orthographic;
}

glm::mat4 Camera::getViewMatrix() const {
	return glm::lookAt(_transform._position, _transform._position + _transform.forward(), _transform.up());
}

glm::mat4 Camera::getProjectionMatrix() const {
	switch (_cameraType) {
	case Perspective:
		return glm::perspective(glm::radians(_fov), 16.0f / 9.0f, _near, _far);
	case Orthographic:
		return glm::ortho(_left, _right, _bottom, _top, _near, _far);
	}
}
