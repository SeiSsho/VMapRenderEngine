#include "Database.h"

void vmap::osm::Database::addNode(const long long& id, std::shared_ptr<vmap::osm::Node> node) {
	_nodes.try_emplace(id, node);
}

const std::unordered_map<long long, std::shared_ptr<vmap::osm::Node>>& vmap::osm::Database::nodes() const {
	return _nodes;
}

std::unique_ptr<vmap::osm::Database> vmap::osm::Database::_instance = nullptr;
vmap::osm::Database& vmap::osm::Database::instance() {
	if (!_instance) {
		_instance = std::make_unique<Database>();
	}
	return *_instance;
}
