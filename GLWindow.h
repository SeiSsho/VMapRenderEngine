#pragma once

#include "OpenGLCore.h"
#include <string_view>

#include "Component/Camera.h"

class GLWindow
{
public:
	class WindowInfo {
		friend class GLWindow;
	private:
		const char* _title;
		int _width = 0, _height = 1;

	public:
		void updateSize(const int& width, const int& height);
		float getAspect() const { return _width * 1.0f / _height; }
		int getWidth() const { return _width; }
		int getHeight() const { return _height; }
	};

private:
	GLFWwindow* _glfwWindow = nullptr;
	WindowInfo _windowInfo;

public:
	GLWindow(std::string_view title, const int& width, const int& height, const bool& useVsync = false);
	~GLWindow();

	void run();

	std::shared_ptr<Camera> camera;
private:
	void inputHandle();
};

