#pragma once

#include "../OpenGLCore.h"
#include "VertexBufferObject.h"
#include <span>

class VertexAttributeObject
{
private:
	GLuint _id;
public:
	enum Type {
		Bool	= GL_BOOL,
		Int		= GL_INT,
		Byte	= GL_BYTE,
		Float	= GL_FLOAT,
		Double	= GL_DOUBLE
	};

	VertexAttributeObject();
	~VertexAttributeObject();
	inline void link(const VertexBufferObject& vbo,
					 const unsigned int& layout,
					 const unsigned int& numComponents,
					 const Type& type,
					 const GLsizeiptr& stride,
					 void* offset,
					 const bool& normalized = false);
	/*
	 * @brief liên kết các vbo theo thứ tự tăng dần từ 0 -> size - 1, kiểu dữ liệu quy định là float[3],
	 * @todo customize sao cho linh hoạt hơn
	 */
	inline void link(std::span<VertexBufferObject> vbos);
	inline void bind() const;
	inline void unbind() const;
};

