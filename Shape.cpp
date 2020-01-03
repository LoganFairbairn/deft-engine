#include "Shape.h"

namespace Deft
{
	//Sets the color for all vertices in the primitive.
	void Shape::SetColor(ColorRGBA8 color)
	{
		for (unsigned int i = 0; i < m_Vertices.size(); i++)
		{
			m_Vertices[i].SetColor(color.r, color.g, color.b, color.a);
		}
	}

	//Sets the depth for all vertices in the primitive.
	void Shape::SetDepth(float depth)
	{
		for (unsigned int i = 0; i < m_Vertices.size(); i++)
		{
			m_Vertices[i].position.z = depth;
		}
	}

	void Shape::SetGLDrawMode(GLDrawMode GLDrawMode)
	{
		m_GLDrawMode = GLDrawMode;
	}
	
	float Shape::GetDepth()
	{
		return m_Vertices[0].position.z;
	}
}