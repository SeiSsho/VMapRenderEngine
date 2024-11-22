#pragma once

#include "Node.h"

namespace vmap::osm {
	class Node;
	class Way {
		friend class Handler;
		friend class Database;
		friend class GLWindow;
		std::vector<std::shared_ptr<Node>> _nodes;
		std::unordered_map<std::string, std::string> _tags;
	public:
		Way();

		const std::unordered_map<std::string, std::string>& tags() const;
		std::optional<std::string> getTagValue(std::string key) const;
	};
}