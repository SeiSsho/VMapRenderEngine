// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "GLWindow.h"
#include <memory>
#include "Asset/Scene.h"
#include "GLObject/Shader.h"
#include "OSM/Database.h"

#include <osmium/io/any_input.hpp>
#include <osmium/visitor.hpp>
#include <osmium/io/pbf_input.hpp>

#include "OSM/Handler.h"

int main()
{
	vmap::osm::Database::instance();
	osmium::io::Reader reader(osmium::io::File("C:/Users/tts/Downloads/map (2).osm"));
	osmium::apply(reader, vmap::osm::Handler());
#ifdef NDEBUG
	spdlog::set_level(spdlog::level::info);
#else
	spdlog::set_level(spdlog::level::trace);
#endif
	std::unique_ptr<GLWindow> window = std::make_unique<GLWindow>("hai", 1920, 1080, true);
	window->run();
	return 0;
}
