#include "Shader.h"

#include <iostream>

Shader::Shader(std::string_view vertexSrc, std::string_view fragmentSrc) {
	int success;
	char infoLog[512];

	auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* src = vertexSrc.data();
	glShaderSource(vertexShader, 1, &src, nullptr);
	glCompileShader(vertexShader);
	
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		spdlog::error("ERROR::SHADER::VERTEX::COMPILATION_FAILED", infoLog);
	}

	auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	src = fragmentSrc.data();
	glShaderSource(fragmentShader, 1, &src, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		spdlog::error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED", infoLog);
	}

	_id = glCreateProgram();
	glAttachShader(_id, vertexShader);
	glAttachShader(_id, fragmentShader);
	glLinkProgram(_id);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	spdlog::trace("[OpenGL]: Create shader {}.", _id);
}

Shader::~Shader() {
	if (_id) {
		//spdlog::trace("[OpenGL]: Delete shader {}.", _id);
		glDeleteProgram(_id);
	}
}

void Shader::bind() const {
	glUseProgram(_id);
}

std::unique_ptr<ShaderRegistry> ShaderRegistry::_instance = nullptr;

ShaderRegistry& ShaderRegistry::instance() {
	if (!_instance) {
		_instance = std::make_unique<ShaderRegistry>();
		const char* vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"uniform mat4 projection;\n"
			"uniform mat4 view;      \n"
			"uniform mat4 model;     \n"
			"void main()\n"
			"{\n"
			"   gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
			"}\0";
		const char* fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(0.0, 1.0, 0.0, 1.0f);\n"
			"}\n\0";
		_instance->_shaders.try_emplace("Default", std::make_shared<Shader>(vertexShaderSource, fragmentShaderSource));
	}
	return *_instance;
}

std::shared_ptr<Shader> ShaderRegistry::getDefaultShader() {
	return ShaderRegistry::instance()._shaders["Default"];
}

std::optional<std::shared_ptr<Shader>> ShaderRegistry::getShader(std::string_view name) {
	auto&& shaderList = instance()._shaders;
	auto it = shaderList.find(name);
	if (it != shaderList.end()) {
		return it->second;
	}
	return {};
}
