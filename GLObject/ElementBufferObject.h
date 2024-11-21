#pragma once

#include "../OpenGLCore.h"

#include <span>

class ElementBufferObject
{
private:
	GLuint _id;
	
public:
	ElementBufferObject(ElementBufferObject&& other) = delete;
	ElementBufferObject(const ElementBufferObject& other) = delete;

	ElementBufferObject(unsigned int* indices, const GLsizeiptr& size, const GLCore::Usage& usage = GLCore::Usage::StaticDraw);
	ElementBufferObject(std::span<unsigned int> indices, const GLCore::Usage& usage = GLCore::Usage::StaticDraw);
	~ElementBufferObject();

	inline void bind() const;
	inline void unbind() const;
};

