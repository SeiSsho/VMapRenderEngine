#pragma once

#include <string_view>
#include <list>
#include "../GameObject.h"

class Scene
{
private:
	std::string_view _name;
	std::list<std::shared_ptr<GameObject>> _root;
public:
	Scene(std::string_view name);
};

