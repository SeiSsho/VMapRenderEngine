#pragma once

#include <unordered_map>
#include <memory>
#include <list>
#include <string_view>
#include <mutex>
#include "Node.h"

namespace vmap::osm {
	class Node;
	class Way;
	class Database {
		friend class Handler;


	private:
		std::unordered_map<long long, std::shared_ptr<vmap::osm::Node>> _nodes;
		std::unordered_map<long long, std::shared_ptr<vmap::osm::Way>> _ways;
		static std::unique_ptr<Database> _instance;
	public:
		Database(const Database&) = delete;
		Database(Database&&) = delete;
		Database& operator=(const Database&) = delete;
		~Database() = default;
		Database() = default;

		void addNode(const long long& id, std::shared_ptr<vmap::osm::Node> node);

		const std::unordered_map<long long, std::shared_ptr<vmap::osm::Node>>& nodes() const;
		const std::unordered_map<long long, std::shared_ptr<vmap::osm::Way>>& ways() const;

		static Database& instance();

	};
}
