#pragma once
#include "Shape.h"

namespace Deft
{
	//This class allows a simple line to be drawn to the screen.
	//A line is a primitive mesh.
	class Line : public Shape
	{
	public:
		Line(GLVec2 pointA, GLVec2 pointB, ColorRGBA8 color, float depth);
	};
}