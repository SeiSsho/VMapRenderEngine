#include "VertexBufferObject.h"

VertexBufferObject::VertexBufferObject(GLfloat* vertices, const GLsizeiptr& size, const GLCore::Usage& usage)
{
	spdlog::trace("[OpenGL]: Generate vertex buffer array {}", _id);
	glGenBuffers(1, &_id);
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
}

VertexBufferObject::VertexBufferObject(std::span<float> vertices, const GLCore::Usage& usage)
{
	spdlog::trace("[OpenGL]: Generate vertex buffer array {}", _id);
	glGenBuffers(1, &_id);
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size(), vertices.data(), usage);
}

VertexBufferObject::~VertexBufferObject()
{
	if (_id)
		spdlog::trace("[OpenGL]: Delete vertex buffer array {}", _id);
		glDeleteBuffers(1, &_id);
}

void VertexBufferObject::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void VertexBufferObject::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
