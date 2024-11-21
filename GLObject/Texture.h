#pragma once

#include "../OpenGLCore.h"

class Texture2D
{
private:
	GLuint _id;

public:
	Texture2D(std::string_view imgPath,
		const GLCore::Format& format,
		const GLCore::Type& type);
	~Texture2D();

	void bind() const;
	inline void unbind() const;
};

