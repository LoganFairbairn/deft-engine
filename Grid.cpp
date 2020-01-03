#include "Grid.h"

namespace Deft
{
	Grid::Grid(GLVec2 position, GLVec2 size, GLfloat spacing, ColorRGBA8 color, float depth)
	{
		float verticalLines = round(size.x / spacing);
		float horizontalLines = round(size.y / spacing);

		//Set vertices.
		m_Vertices.resize((verticalLines + horizontalLines) * 2);
		

		unsigned int currentVertex = 0;
		for (int i = 0; i < verticalLines; i++)
		{
			m_Vertices.at(currentVertex++).SetPosition(position.x + (spacing * i), position.y);
			m_Vertices.at(currentVertex++).SetPosition(position.x + (spacing * i), position.y + size.y);
		}

		for (int i = 0; i < horizontalLines; i++)
		{
			m_Vertices.at(currentVertex++).SetPosition(position.x, position.y + (spacing * i));
			m_Vertices.at(currentVertex++).SetPosition(position.x + size.x, position.y + (spacing * i));
		}

		//Set the indices.
		m_Indices.resize((verticalLines + horizontalLines) * 2);
		for (int i = 0; i < m_Vertices.size(); i++)
		{
			m_Indices.at(i) = i;
		}

		//Set the color, depth, and draw mode.
		SetColor(color);
		SetDepth(depth);
		SetGLDrawMode(GLDrawMode::LINES);
	}
}