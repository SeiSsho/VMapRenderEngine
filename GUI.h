#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <vector>
#include <functional>

class GUI
{
	friend class GLWindow;
	std::vector <std::function<void()>> _uiBlock;
public:
	GUI(GLFWwindow* window);
	~GUI();

	void addRenderBlock(std::function<void()> block);
	void draw();
	void renderDrawData();
};

