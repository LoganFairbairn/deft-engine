#include "Line.h"

namespace Deft
{
	Line::Line(GLVec2 pointA, GLVec2 pointB, ColorRGBA8 color, float depth)
	{
		//Set the vertices.
		m_Vertices.resize(2);
		m_Vertices[0].SetPosition(pointA.x, pointA.y);
		m_Vertices[1].SetPosition(pointB.x, pointB.y);

		//Set the indicies.
		m_Indices.resize(2);
		m_Indices[0] = 0;
		m_Indices[1] = 1;

		//Set color, depth and draw mode.
		SetColor(color);
		SetDepth(depth);
		SetGLDrawMode(GLDrawMode::LINES);
	}
}