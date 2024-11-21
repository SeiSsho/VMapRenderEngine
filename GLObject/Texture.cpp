#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <future>

Texture2D::Texture2D(std::string_view imgPath,
	const GLCore::Format& format, const GLCore::Type& type) {

	int widthImg, heightImg, numChs;
	stbi_set_flip_vertically_on_load(true);
	std::future<unsigned char*> imgDataFuture = std::async([=]() mutable
		{
			return stbi_load(imgPath.data(), &widthImg, &heightImg, &numChs, 0);
		});
	std::async(stbi_load, imgPath.data(), &widthImg, &heightImg, &numChs, 0).get();

	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	auto imageData = imgDataFuture.get();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, type, imageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	delete imageData;
}

Texture2D::~Texture2D() {
	glDeleteTextures(1, &_id);
}

void Texture2D::bind() const {
	glBindTexture(GL_TEXTURE_2D, _id);
}

inline void Texture2D::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}
