#pragma once
#include <vector>
#include "Vertex.h"
#include "GLDrawMode.h"

namespace Deft
{
	//This is a class that is used to define simple shapes drawn using OpenGL primitive draw modes.
	class Shape
	{
	public:
		Shape() {};
		~Shape() {};

		//Setters.
		void SetColor(ColorRGBA8 color);
		void SetDepth(float depth);
		void SetGLDrawMode(GLDrawMode GLDrawMode);

		//Getters.
		GLDrawMode GetGLDrawMode() { return m_GLDrawMode; }
		float GetDepth();

		std::vector<PrimitiveVertex> m_Vertices;	//Vertices for this shape.
		std::vector<GLuint> m_Indices;				//Indices for this shape.

	private:
		GLDrawMode m_GLDrawMode;		//OpenGL primitive Draw mode for this shape.
	};
}