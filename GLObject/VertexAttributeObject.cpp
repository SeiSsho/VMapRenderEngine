#include "VertexAttributeObject.h"
#include <spdlog/spdlog.h>

VertexAttributeObject::VertexAttributeObject()
{
	glGenVertexArrays(1, &_id);
	spdlog::trace("[OpenGL]: Generate vertex attribute array {}", _id);
}

VertexAttributeObject::~VertexAttributeObject()
{
	if (_id)
		spdlog::trace("[OpenGL]: Delete vertex array {}", _id);
		glDeleteVertexArrays(1, &_id);
}

void VertexAttributeObject::link(const VertexBufferObject& vbo, 
										const unsigned int& layout, 
										const unsigned int& numComponents, 
										const GLCore::Type& type, 
										const GLsizeiptr& stride, 
										void* offset,
										const bool& normalized)
{
	vbo.bind();
	glVertexAttribPointer(layout, numComponents, type, normalized, stride, offset);
	glEnableVertexAttribArray(layout);
	vbo.unbind();
}

void VertexAttributeObject::bind() const
{
	glBindVertexArray(_id);
}

void VertexAttributeObject::unbind() const
{
	glBindVertexArray(0);
}
