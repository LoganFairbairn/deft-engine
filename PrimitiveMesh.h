#pragma once
#include <GL\glew.h>
#include <vector>

#include "Vertex.h"

namespace Deft
{
	//This is a mesh for drawing primitives (squares, circles, lines etc...).
	//It needs an array of vertice position and color data to draw.
	class PrimitiveMesh
	{
	public:
		PrimitiveMesh();
		virtual ~PrimitiveMesh();

		void Initialize(std::vector<PrimitiveVertex> vertices, std::vector<GLubyte> indices);
		void DrawMesh();
		void DrawLines();
		void DrawLineStrip();
		void SetMeshColor(ColorRGBA8 color);
		void SetDepth(float depth);

	protected:
		void UpdateMeshData();

		//TODO: Move the buffers to the renderer (increase performance).
		GLuint m_VAO;				//This is the OpenGL vertex attribute object ID.
		GLuint m_VBO;				//This is the OpenGL vertex buffer object ID.
		GLuint m_IndexBuffer;
		GLuint m_ArrayBufferSize;
		GLuint m_IndexBufferSize;

		

		std::vector<GLubyte> m_Indices;	
		std::vector<PrimitiveVertex> m_Vertices;		//These are the 2D points that define the mesh.
		ColorRGBA8 m_Color;
	};
}