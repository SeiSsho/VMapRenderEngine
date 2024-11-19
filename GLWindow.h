#pragma once

#include "OpenGLCore.h"
#include <memory>
#include <string_view>
class GLWindow
{
public:
	class WindowInfo {
	private:
		const char* _title;
		int _width, _height;
	public:
		void updateSize(const int& width, const int& height);
	};

private:
	GLFWwindow* _glfwWindow = nullptr;
	WindowInfo _windowInfo;

public:
	GLWindow(std::string_view title, const int& width, const int& height, const bool& useVsync = false);
	~GLWindow();

	void run();
};
