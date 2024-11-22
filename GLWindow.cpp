#include "GLWindow.h"

#include <glm/gtc/type_ptr.hpp>

#include "Input.h"
#include "Mesh.h"
#include "Component/Camera.h"
#include "GLObject/Shader.h"
#include <future>
#include <iostream>

#include "OSM/Database.h"
#include "OSM/Way.h"

GLWindow::GLWindow(std::string_view title, const int& width, const int& height, const bool& useVsync)
{
	_windowInfo._width = width;
	_windowInfo._height = height;
	if (!glfwInit()) {
		throw std::exception("Failed to initialize GLFW.");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	_glfwWindow = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
	if (!_glfwWindow) {
		throw std::exception("Failed to initialize GLFWwindow.");
	}

	glfwMakeContextCurrent(_glfwWindow);
	glfwSetWindowUserPointer(_glfwWindow, &_windowInfo);

	glfwSetFramebufferSizeCallback(_glfwWindow, [](GLFWwindow* window, int width, int height) -> void {
		auto windowInfo = static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));
		windowInfo->updateSize(width, height);
		});
	glfwSetCursorPosCallback(_glfwWindow, [](GLFWwindow* window, double x, double y) -> void {
		Input::getInstance().getMouse().setPosition(x, y);
		});

	glfwSwapInterval(useVsync);

	if (glewInit() != GLEW_OK) {
		throw std::exception("Failed to initialize GLEW.");
	}

	Transform cameraTransform;
	//cameraTransform._position = { 210, 1055, -3 };
	cameraTransform._position = { 1055, 210, 3 };
	cameraTransform._rotation = glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
	camera = std::make_shared<Camera>(cameraTransform, 45.0f, _windowInfo.getAspect(), 0.00001f, 1000000.0f);
	//camera = std::make_shared<Camera>(cameraTransform, 
		//-1 * _windowInfo.getAspect() / 2, 1 * _windowInfo.getAspect() / 2, 1.0 / 2, -1.0 / 2, 0.00001f, 1000000.0f);
}

GLWindow::~GLWindow()
{
	glfwDestroyWindow(_glfwWindow);
	glfwTerminate();
}

void GLWindow::run()
{
	auto vao = std::make_unique<VertexAttributeObject>();
	unsigned long long size = 0;
	{
		std::vector<float> arr;
		for (const auto& node : vmap::osm::Database::instance().nodes()) {
			arr.push_back(static_cast<float>(node.second->lat() * 10));
			arr.push_back(static_cast<float>(node.second->lon() * 10));
			arr.push_back(0.0f);
		}
		//int c = 0;
		//for (const auto& way : vmap::osm::Database::instance().ways()) {
		//	//auto otp = way.second->getTagValue("highway");
		//	//if (otp.has_value()) {
		//	for (int i = 0; i < way.second->_nodes.size() - 2; i++) {
		//		auto nodes = way.second->_nodes;
		//			auto lat = static_cast<float>(nodes[i]->lat() * 10);
		//			auto lon = static_cast<float>(nodes[i]->lon() * 10);
		//			arr.push_back(lat);
		//			arr.push_back(lon);
		//			arr.push_back(0.0f);
		//			lat = static_cast<float>(nodes[i + 1]->lat() * 10);
		//			lon = static_cast<float>(nodes[i + 1]->lon() * 10);
		//			arr.push_back(lat);
		//			arr.push_back(lon);
		//			arr.push_back(0.0f);
		//		}
		//	//}
		//	c++;
		//}

		VertexBufferObject vbo = VertexBufferObject(arr);
		vao->bind();
		vbo.bind();
		vao->link(vbo, 0, 3, GLCore::Type::Float, sizeof(float) * 3, 0);
		vbo.unbind();
		vao->unbind();
		size = arr.size();
	}

	auto shader = ShaderRegistry::getDefaultShader();
	auto shaderId = shader->getId();
	GLuint modelLoc = glGetUniformLocation(shaderId, "model");
	GLuint viewLoc = glGetUniformLocation(shaderId, "view");
	GLuint projLoc = glGetUniformLocation(shaderId, "projection");

	Transform objectTransform;
	Mesh mesh(std::move(vao), size, GLCore::Primitive::Points);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(_glfwWindow)) {
		glfwPollEvents();
		inputHandle();

		//spdlog::info("view port {} {}", _windowInfo.getWidth(), _windowInfo.getHeight());
		glfwSwapBuffers(_glfwWindow);

		glViewport(0, 0, _windowInfo.getWidth(), _windowInfo.getHeight());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glLineWidth(1);
		glPointSize(2.0);
		shader->bind();
		//spdlog::info("Camera pos {} {} {}", camera->_transform._position.x, camera->_transform._position.y, camera->_transform._position.z);
		glUniformMatrix4fv(modelLoc, 1, false, glm::value_ptr(objectTransform.matrixTransform()));
		glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(camera->getViewMatrix()));
		glUniformMatrix4fv(projLoc, 1, false, glm::value_ptr(camera->getProjectionMatrix()));

		mesh.draw();
	}
}

void GLWindow::inputHandle() {
	if (glfwGetKey(_glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(_glfwWindow, true);
	if (glfwGetKey(_glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
		camera->_transform._position += camera->_transform.forward() * 0.1f;
	if (glfwGetKey(_glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
		camera->_transform._position += camera->_transform.forward() * -0.1f;
	if (glfwGetKey(_glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
		camera->_transform._position += camera->_transform.right() * -0.1f;
	if (glfwGetKey(_glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
		camera->_transform._position += camera->_transform.right() * 0.1f;
	if (glfwGetKey(_glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera->_transform._position += camera->_transform.up() * -0.1f;
	if (glfwGetKey(_glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->_transform._position += camera->_transform.up() * +0.1f;
}

void GLWindow::WindowInfo::updateSize(const int& width, const int& height)
{
	_width = width;
	_height = height;
	spdlog::trace("Resize window {}x{}", width, height);
}
