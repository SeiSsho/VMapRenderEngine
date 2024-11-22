#include "Node.h"

#include <limits>

vmap::osm::Node::Node() : _x(INT_MAX), _y(INT_MAX) {
}

vmap::osm::Node::Node(const int32_t& x, const int32_t& y)
	: _x(x), _y(y) {
}

const std::unordered_map<std::string, std::string>& vmap::osm::Node::tags() const {
	return _tags;
}

std::optional<std::string> vmap::osm::Node::getTagValue(std::string key) const {
	if (_tags.find(key) != _tags.end()) {
		return _tags.at(key);
	}
	return {};
}
