// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "GLWindow.h"
#include <memory>
#include "Asset/Scene.h"
int main()
{
#ifdef NDEBUG
	spdlog::set_level(spdlog::level::info);
#else
	spdlog::set_level(spdlog::level::trace);
#endif
	std::unique_ptr<GLWindow> window = std::make_unique<GLWindow>("hai", 640, 480);
	window->run();
	return 0;
}
