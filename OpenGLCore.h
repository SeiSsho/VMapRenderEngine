#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

namespace GLCore {
	enum Type {
		Bool			= GL_BOOL,
		Int				= GL_INT,
		UByte			= GL_UNSIGNED_BYTE,
		Byte			= GL_BYTE,
		Float			= GL_FLOAT,
		Double			= GL_DOUBLE
	};

	enum Usage {
		StaticDraw		= GL_STATIC_DRAW,
		DynamicDraw		= GL_DYNAMIC_DRAW,
		StreamDraw		= GL_STREAM_DRAW
	};

	enum Format {
		Red				= GL_RED,
		Green			= GL_GREEN,
		Blue			= GL_BLUE,
		RG				= GL_RG,
		RGB				= GL_RGB,
		RGBA			= GL_RGBA
	};

	enum Primitive {
		Points			= GL_POINTS,
		Lines			= GL_LINES,
		LineStrip		= GL_LINE_STRIP,
		LineLoop		= GL_LINE_LOOP,
		Polygon			= GL_POLYGON,
		Triangles		= GL_TRIANGLES,
		TriangleStrip	= GL_TRIANGLE_STRIP,
		TriangleFan		= GL_TRIANGLE_FAN,
		Quads			= GL_QUADS,
		QuadStrip		= GL_QUAD_STRIP
	};
};
