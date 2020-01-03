#include "Rectangle.h"

namespace Deft
{
	Rectangle::Rectangle(Vec2 size, Vec2 position, ColorRGBA8 color, float depth)
	{
		//Set vertices.
		m_Vertices.resize(4);
		m_Vertices[0].SetPosition(position.x, position.y);
		m_Vertices[1].SetPosition(position.x, position.y + size.y);
		m_Vertices[2].SetPosition(position.x + size.x, position.y + size.y);
		m_Vertices[3].SetPosition(position.x + size.x, position.y);

		//Set indices.
		m_Indices.resize(6);
		m_Indices[0] = 0;
		m_Indices[1] = 1;
		m_Indices[2] = 2;
		m_Indices[3] = 0;
		m_Indices[4] = 2;
		m_Indices[5] = 3;

		//Set color, depth and draw mode.
		SetColor(color);
		SetDepth(depth);
		SetGLDrawMode(GLDrawMode::TRIANGLES);
		
		//Set the size for the shape.
		SetPosition(position);
		SetSize(size);
	}

	//Sets the position for the shape by updating it's vertices.
	void Rectangle::SetPosition(Vec2 position)
	{
		m_Position = position;

		m_Vertices[0].SetPosition(position.x, position.y);
		m_Vertices[1].SetPosition(position.x, position.y + m_Size.y);
		m_Vertices[2].SetPosition(position.x + m_Size.x, position.y + m_Size.y);
		m_Vertices[3].SetPosition(position.x + m_Size.x, position.y);
	}

	//Sets the size of the shape by updatign it's vertices.
	void Rectangle::SetSize(Vec2 size)
	{
		m_Size = size;

		m_Vertices[0].SetPosition(m_Position.x, m_Position.y);
		m_Vertices[1].SetPosition(m_Position.x, m_Position.y + size.y);
		m_Vertices[2].SetPosition(m_Position.x + size.x, m_Position.y + size.y);
		m_Vertices[3].SetPosition(m_Position.x + size.x, m_Position.y);
	}
}
