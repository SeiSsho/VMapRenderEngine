#include "Handler.h"
#include <string>

#include "Way.h"

void vmap::osm::Handler::node(const osmium::Node& node) {
	auto mnode = std::make_shared<vmap::osm::Node>(node.location().x(), node.location().y());
	for (const auto & tag : node.tags()) {
		mnode->_tags.insert({ std::string(tag.key()), std::string(tag.value())});
	}
	Database::instance()._nodes.try_emplace(node.id(), mnode);
}

void vmap::osm::Handler::way(const osmium::Way& way) {
	auto mway = std::make_shared<vmap::osm::Way>();
	// add tag
	for (const auto& tag : way.tags()) {
		mway->_tags.insert({ std::string(tag.key()), std::string(tag.value()) });
	}
	for (const auto& node : way.nodes()) {
		mway->_nodes.push_back(Database::instance()._nodes.at(node.ref()));
	}
	Database::instance()._ways.try_emplace(way.id(), mway);
}

void vmap::osm::Handler::relation(const osmium::Relation& relation) {
}
