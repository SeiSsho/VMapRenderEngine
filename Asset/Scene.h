#pragma once

#include <string_view>

class Scene
{
private:
	std::string_view _name;
public:
	Scene(std::string_view name);
};

