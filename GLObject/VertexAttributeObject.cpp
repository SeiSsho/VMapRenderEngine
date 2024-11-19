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
		glDeleteVertexArrays(1, &_id);
		spdlog::trace("[OpenGL]: Delete vertex array {}", _id);
}

inline void VertexAttributeObject::link(const VertexBufferObject& vbo, 
										const unsigned int& layout, 
										const unsigned int& numComponents, 
										const Type& type, 
										const GLsizeiptr& stride, 
										void* offset,
										const bool& normalized)
{
	vbo.bind();
	glVertexAttribPointer(layout, numComponents, type, normalized, stride, offset);
	vbo.unbind();
}

inline void VertexAttributeObject::link(std::span<VertexBufferObject> vbos)
{
	unsigned int index = 0;
	for (const auto& vbo : vbos) {
		vbo.bind();
		glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(static_cast<GLuint>(index));
		++index;
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline void VertexAttributeObject::bind() const
{
	glBindVertexArray(_id);
}

inline void VertexAttributeObject::unbind() const
{
	glBindVertexArray(0);
}
