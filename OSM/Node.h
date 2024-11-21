#pragma once

#include <unordered_map>
#include <memory>
#include <list>
#include <string_view>

namespace vmap::osm {
	class Node {
		friend class Database;
		friend class Handler;
		static constexpr double COORDINATE_PRECISION = 10000000;

		int32_t _x, _y;
		std::unordered_map<const char*, const char*> _tags;
	public:
		Node();
		Node(const int32_t& x, const int32_t& y);
		Node(Node&) = delete;
		Node(Node&&) = delete;
		Node operator=(const Node&) const = delete;

		int32_t x() const { return _x; }
		int32_t y() const { return _y; }

		double lat() const { return static_cast<double>(_x) / COORDINATE_PRECISION; }
		double lon() const { return static_cast<double>(_y) / COORDINATE_PRECISION; }
	};
};
