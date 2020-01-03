#pragma once
#include <GL/glew.h>
#include <glm\glm.hpp>
#include <vector>
#include <algorithm>		//Included here for stable_sort.

#include "Vertex.h"
#include "Renderer.h"
#include "Shape.h"
#include "GLDrawMode.h"

namespace Deft
{
	class Renderer;

	struct ShapeBatch
	{
		ShapeBatch(GLDrawMode mode) : GLDrawMode(mode) {}

		GLDrawMode GLDrawMode;		//OpenGL primitive draw mode for this batch.
		unsigned int indices;		//Number of indices that will be rendered with this draw mode.
	};

	//Batches shapes together for more efficient drawing.
	class ShapeRenderer
	{
	public:
		ShapeRenderer(Renderer* renderer);
		~ShapeRenderer();

		void StartBatching();
		void BatchShape(Shape* shape);
		void EndBatching();

		//Getters.
		unsigned int GetVerticesRendering() { return m_VerticesRendering; };
		unsigned int GetIndicesRendering() { return m_IndicesRendering; };

	private:
		void UpdateGPU();
		void DrawShapeBatches();

		static bool CompareGLDrawMode(Shape * a, Shape* b);
		static bool CompareDepth(Shape* a, Shape* b);

		Renderer* m_Renderer;

		GLuint m_VBO = 0;				//This is the OpenGL buffer object ID.
		GLuint m_VAO = 0;				//This is the OpenGL array object ID.
		GLuint m_IndexBuffer;			//This is the OpenGL index buffer ID.
		GLuint m_ArrayBufferSize;		//Size of the array buffer sent to the GPU.
		GLuint m_IndexBufferSize;		//Size of the indice buffer sent to the GPU.

		std::vector<GLuint> m_Indices;				//Indices that will be updated on the GPU.
		std::vector<PrimitiveVertex> m_Vertices;	//Vertices that will be updates on the GPU.

		unsigned int m_VerticesToRender;
		unsigned int m_IndicesToRender;

		std::vector<Shape*> m_Shapes;				//A list of shapes that will be drawn to the screen.
		std::vector<ShapeBatch> m_ShapeBatches;		//Number of indices that are drawn in this batch.

		//Performance tracking variables.
		unsigned int m_VerticesRendering = 0;
		unsigned int m_IndicesRendering = 0;
	};
}