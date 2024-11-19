#pragma once
#include "../OpenGLCore.h"
#include <span>

class VertexBufferObject
{
private:
	GLuint _id;
	VertexBufferObject(VertexBufferObject&& other) = delete;
	VertexBufferObject(const VertexBufferObject& other) = delete;
public:
	enum Usage {
		StaticDraw = GL_STATIC_DRAW,
		DynamicDraw = GL_DYNAMIC_DRAW,
		StreamDraw = GL_STREAM_DRAW
	};

	VertexBufferObject(GLfloat* vertices, const GLsizeiptr& size, const Usage& usage = StaticDraw);
	VertexBufferObject(std::span<float> vertices, const Usage& usage = StaticDraw);
	~VertexBufferObject();

	inline void bind() const;
	inline void unbind() const;
};

