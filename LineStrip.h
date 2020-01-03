#pragma once
#include <vector>
#include "Shape.h"

namespace Deft
{
	//This class allows a simple line to be drawn to the screen.
	//A line is a primitive mesh.
	class LineStrip : public Shape
	{
	public:
		LineStrip(std::vector<Vec2> points, ColorRGBA8 color, float depth);
	};
}