#include "Input.h"

Input Input::getInstance()
{
    static Input instance;
    return instance;
}

KeyBoard& Input::getKeyBoard()
{
    return _keyBoard;
}

Mouse& Input::getMouse()
{
    return _mouse;
}

Mouse::Mouse()
{
}

double Mouse::getX() const
{
    return _x;
}

double Mouse::getY() const
{
    return _y;
}

void Mouse::setX(const double& x)
{
    _x = x;
}

void Mouse::setY(const double& y)
{
    _y = y;
}

std::pair<double, double> Mouse::getPosition() const
{
    return { _x, _y };
}

void Mouse::setPosition(const double& x, const double& y)
{
    _x = x;
    _y = y;
}
