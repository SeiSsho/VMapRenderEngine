#include "GLWindow.h"
#include "Input.h"

GLWindow::GLWindow(std::string_view title, const int& width, const int& height, const bool& useVsync)
{
	if (!glfwInit()) 
		spdlog::critical("Failed to initialize GLFW.");
		throw std::exception("Failed to initialize GLFW.");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	_glfwWindow = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
	if (!_glfwWindow)
		spdlog::critical("Failed to initialize GLFWwindow.");
		throw std::exception("Failed to initialize GLFWwindow.");

	glfwMakeContextCurrent(_glfwWindow);
	glfwSetWindowUserPointer(_glfwWindow, &_windowInfo);

	glfwSetFramebufferSizeCallback(_glfwWindow, [] (GLFWwindow* window, int width, int height) -> void {
		auto windowInfo = static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));
		windowInfo->updateSize(width, height);
		});
	glfwSetCursorPosCallback(_glfwWindow, [](GLFWwindow* window, double x, double y) -> void {
		Input::getInstance().getMouse().setPosition(x, y);
		});

	glfwSwapInterval(useVsync);

	if (glewInit() != GLEW_OK)
		spdlog::critical("Failed to initialize GLEW.");
		throw std::exception("Failed to initialize GLEW.");
}

GLWindow::~GLWindow()
{
	glfwDestroyWindow(_glfwWindow);
	glfwTerminate();
}

void GLWindow::run()
{
	while (!glfwWindowShouldClose(_glfwWindow)) {
		glClear(GL_COLOR_BUFFER_BIT);


		glfwSwapBuffers(_glfwWindow);
		glfwPollEvents();
	}
}

void GLWindow::WindowInfo::updateSize(const int& width, const int& height)
{
	_width = width;
	_height = height;
	spdlog::trace("Resize window {}x{}", width, height);
}
