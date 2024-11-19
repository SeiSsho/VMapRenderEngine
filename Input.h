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
	inline double getX() const;
	inline double getY() const;
	inline void setX(const double& x);
	inline void setY(const double& y);

	/*
	 * @return x, y position of mouse.
	 */
	inline std::pair<double, double> getPosition() const;
	inline void setPosition(const double& x, const double& y);
};

class Input
{
	friend class GLWindow;
public:
	static Input getInstance();

	inline KeyBoard& getKeyBoard();
	inline Mouse& getMouse();
private:
	KeyBoard _keyBoard;
	Mouse _mouse;
};

