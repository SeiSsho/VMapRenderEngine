#pragma once

#include "../OpenGLCore.h"
#include <filesystem>
class Shader
{
private:
	GLuint _id;

public:
	Shader(std::string_view vertexSrc, std::string_view fragmentSrc);
	~Shader();

	void bind() const;
	unsigned int getId() const { return _id; }
};

class ShaderRegistry {
private:
	std::unordered_map<std::string_view, std::shared_ptr<Shader>> _shaders;

	static std::unique_ptr<ShaderRegistry> _instance;

public:
	ShaderRegistry() = default;
	~ShaderRegistry() = default;
	ShaderRegistry(const ShaderRegistry&) = delete;
	ShaderRegistry(ShaderRegistry&&) = delete;
	ShaderRegistry& operator=(const ShaderRegistry&) = delete;

	static ShaderRegistry& instance();
	static std::shared_ptr<Shader> getDefaultShader();
	static std::optional<std::shared_ptr<Shader>> getShader(std::string_view name);
};
