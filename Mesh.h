#pragma once

#include "GLObject/VertexAttributeObject.h"

#include <memory>
#include <vector>

/**
 * Not support EBO yet
 */
class Mesh {
private:
	std::unique_ptr<VertexAttributeObject> _vao;
	int _size;
	GLCore::Primitive _primitive;
public:
	explicit Mesh(std::unique_ptr<VertexAttributeObject> vao, const int& size, const GLCore::Primitive& primitive);

	/**
	 * Update camera, projection, view matrix to shader before draw
	 */
	void draw() const;
};

