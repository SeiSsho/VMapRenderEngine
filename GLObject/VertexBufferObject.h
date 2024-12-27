#pragma once

#include "../OpenGLCore.h"
#include <span>



class VertexBufferObject
{
private:
	GLuint _id;
	
public:
	VertexBufferObject(VertexBufferObject&& other) = delete;
	VertexBufferObject(const VertexBufferObject& other) = delete;

	explicit VertexBufferObject(GLfloat* vertices, const GLsizeiptr& size, const GLCore::Usage& usage = GLCore::Usage::StaticDraw);
	explicit VertexBufferObject(GLdouble* vertices, const GLsizeiptr& size, const GLCore::Usage& usage = GLCore::Usage::StaticDraw);
	explicit VertexBufferObject(std::span<float> vertices, const GLCore::Usage& usage = GLCore::Usage::StaticDraw);
	explicit VertexBufferObject(std::span<double> vertices, const GLCore::Usage& usage = GLCore::Usage::StaticDraw);
	~VertexBufferObject();

	void bind() const;
	void unbind() const;
};

