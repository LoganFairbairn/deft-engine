#pragma once
#include "Shape.h"
#include "Vector.h"

namespace Deft
{
	//This class allows rectangles to be drawn to the screen.
	//This rectangle is a primitive mesh.
	class Rectangle : public Shape
	{
	public:
		Rectangle(Vec2 size, Vec2 position, ColorRGBA8 color, float depth);

		//Setters.
		void SetPosition(Vec2 position);
		void SetSize(Vec2 size);

	private:
		Vec2 m_Position;
		Vec2 m_Size;
	};
}

