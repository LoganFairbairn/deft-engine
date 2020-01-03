#include "LineStrip.h"

namespace Deft
{
	LineStrip::LineStrip(std::vector<Vec2> points, ColorRGBA8 color, float depth)
	{
		//Set vertices.
		m_Vertices.resize(points.size());

		for (unsigned int i = 0; i < points.size(); i++)
		{
			m_Vertices[i].position.x = points[i].x;
			m_Vertices[i].position.y = points[i].y;
		}

		//Set indicies.
		m_Indices.resize(points.size());

		for (unsigned int i = 0; i < points.size(); i++)
		{
			m_Indices[i] = i;
		}

		//Set color, depth and draw mode.
		SetColor(color);
		SetDepth(depth);
		SetGLDrawMode(GLDrawMode::LINES);
	}
}