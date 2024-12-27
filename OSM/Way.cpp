#include "Way.h"

vmap::osm::Way::Way() {
}

const std::unordered_map<std::string, std::string>& vmap::osm::Way::tags() const {
	return _tags;
}

std::unordered_map<std::string, std::string>& vmap::osm::Way::tags() {
	return _tags;
}

std::optional<std::string> vmap::osm::Way::getTagValue(std::string key) const {
	if (_tags.find(key) != _tags.end()) {
		return _tags.at(key);
	}
	return {};
}
