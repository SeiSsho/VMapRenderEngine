#pragma once

#include <glm/common.hpp>
#include "OpenGLCore.h"
#include "GLWindow.h"

class GLWindow;

class KeyBoard {

};

class Mouse {
	double _x = 0, _y = 0;
public:
	Mouse();
	double getX() const;
	double getY() const;
	void setX(const double& x);
	void setY(const double& y);

	/*
	 * @return x, y position of mouse.
	 */
	std::pair<double, double> getPosition() const;
	void setPosition(const double& x, const double& y);
};

class Input
{
	friend class GLWindow;
public:
	static Input getInstance();

	KeyBoard& getKeyBoard();
	Mouse& getMouse();
private:
	KeyBoard _keyBoard;
	Mouse _mouse;
};

