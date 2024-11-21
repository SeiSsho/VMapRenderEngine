#include "ElementBufferObject.h"

ElementBufferObject::ElementBufferObject(unsigned int* indices, const GLsizeiptr& size, const GLCore::Usage& usage)
{
	glGenBuffers(1, &_id);
	spdlog::trace("[OpenGL]: Generate element buffer array {}", _id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage);
}

ElementBufferObject::ElementBufferObject(std::span<unsigned int> indices, const GLCore::Usage& usage)
{
	glGenBuffers(1, &_id);
	spdlog::trace("[OpenGL]: Generate element buffer array {}", _id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), indices.data(), usage);
}

ElementBufferObject::~ElementBufferObject()
{
	spdlog::trace("[OpenGL]: Delete element buffer array {}", _id);
	glDeleteBuffers(1, &_id);
}

inline void ElementBufferObject::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

inline void ElementBufferObject::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
