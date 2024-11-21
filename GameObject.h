#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <list>
#include <unordered_map>
#include <typeindex>

class Component;
class Transform;
class GameObject
{
private:
	std::unordered_map<std::type_index, std::shared_ptr<Component>> _components;
	std::list<std::shared_ptr<GameObject>> _children;
	std::weak_ptr<GameObject> _parent;
	std::string_view _name;

public:

};

