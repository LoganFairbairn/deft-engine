#include "Circle.h"

namespace Deft
{
	Circle::Circle(Vec2 position, float radius, unsigned int quality, ColorRGBA8 color, float depth)
	{
		//Make sure the quality isn't less than 3.
		if (quality < 3)
		{
			quality = 3;
		}

		if (m_Vertices.size() - 1 < quality)
		{
			m_Vertices.resize(quality + 1);
		}

		//First vertice is always the center.
		m_Vertices[0].SetPosition(position.x, position.y);

		//Set the vertices for the circle.
		int index = 1;
		for (GLfloat i = 0; i < 360.0f; i += (360.0f / quality))
		{
			m_Vertices[index].position.x = (cos(i * (float)0.0174533f) * radius) + position.x;
			m_Vertices[index].position.y = (sin(i * (float)0.0174533f) * radius) + position.y;
			index++;
		}

		//Set the default indices for a circle.
		m_Indices.resize(quality * 3);

		int indice = 0;
		for (int i = 0; i < quality; i++)
		{
			m_Indices.at(indice) = 0;
			indice++;

			m_Indices.at(indice) = i + 1;
			indice++;

			//Set the last indice to 1 (back to the start of the circle).
			if (indice == m_Indices.size() - 1)
			{
				m_Indices.at(indice) = 1;
			}

			else
			{
				m_Indices.at(indice) = i + 2;
				indice++;
			}
		}

		//Set the color, depth, and draw mode.
		SetColor(color);
		SetDepth(depth);
		SetGLDrawMode(GLDrawMode::TRIANGLES);
	}
}
