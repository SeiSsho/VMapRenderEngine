#pragma once

#include <iostream>

#include "Database.h"
#include <memory>
#include <osmium/io/any_input.hpp>
#include <osmium/handler.hpp>

namespace vmap::osm {
	class Database;

	class Handler : public osmium::handler::Handler
	{
	public:
		void node(const osmium::Node& node);
		void way(const osmium::Way& way);
		void relation(const osmium::Relation& relation);
	};

}
