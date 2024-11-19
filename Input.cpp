#include "Input.h"

Input Input::getInstance()
{
    static Input instance;
    return instance;
}

inline KeyBoard& Input::getKeyBoard()
{
    return _keyBoard;
}

inline Mouse& Input::getMouse()
{
    return _mouse;
}

Mouse::Mouse()
{
}

inline double Mouse::getX() const
{
    return _x;
}

inline double Mouse::getY() const
{
    return _y;
}

inline void Mouse::setX(const double& x)
{
    _x = x;
}

inline void Mouse::setY(const double& y)
{
    _y = y;
}

inline std::pair<double, double> Mouse::getPosition() const
{
    return { _x, _y };
}

inline void Mouse::setPosition(const double& x, const double& y)
{
    _x = x;
    _y = y;
}
