#pragma once
#include "Shape.h"

namespace Deft
{
	class Circle : public Shape
	{
	public:
		Circle(Vec2 position, float radius, unsigned int quality, ColorRGBA8 color, float depth);
	};
}

