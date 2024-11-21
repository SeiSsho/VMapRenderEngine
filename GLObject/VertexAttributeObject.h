#pragma once

#include "../OpenGLCore.h"
#include "VertexBufferObject.h"
#include <span>

class VertexAttributeObject
{
private:
	GLuint _id;
public:

	VertexAttributeObject();
	~VertexAttributeObject();
	void link(const VertexBufferObject& vbo,
					 const unsigned int& layout,
					 const unsigned int& numComponents,
					 const GLCore::Type& type,
					 const GLsizeiptr& stride,
					 void* offset,
					 const bool& normalized = false);

	void bind() const;
	void unbind() const;
};

