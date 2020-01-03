#pragma once
#include "Shape.h"

namespace Deft
{
	class Grid : public Shape
	{
	public:
		Grid(GLVec2 position, GLVec2 size, GLfloat spacing, ColorRGBA8 color, float depth);
	};
}