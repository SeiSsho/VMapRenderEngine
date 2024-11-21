#pragma once

#include <memory>

class GameObject;

class Component
{
private:
	std::weak_ptr<GameObject> _gameObject;
};

