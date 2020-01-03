#include <algorithm>		//Included here for stable_sort.
#include "ShapeRenderer.h"

namespace Deft
{
	ShapeRenderer::ShapeRenderer(Renderer* renderer)
	{
		m_Renderer = renderer;

		//Generate a vertex attribute object.
		if (m_VAO == 0)
		{
			glGenVertexArrays(1, &m_VAO);
		}

		//Bind the vertex array object.
		glBindVertexArray(m_VAO);

		//Check that there isn't a vertex buffer object already generated.
		if (m_VBO == 0)
		{
			glGenBuffers(1, &m_VBO);
		}

		//Generate an index buffer.
		glGenBuffers(1, &m_IndexBuffer);

		//Bind the vertex buffer object.
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		//Enable required attribute arrays (for primitives it's position and color).
		//Note: Enabling these vertex attribute arrays casuses overhead, avoid calling these as much as possible.
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		//Define the vertex attributes.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveVertex), (void*)offsetof(PrimitiveVertex, position));
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(PrimitiveVertex), (void*)offsetof(PrimitiveVertex, color));

		//Unbind the vertex array object (this also unbinds the vertex buffer object and disables all vertex attribute arrays).
		glBindVertexArray(0);

		//Set the start buffer size.
		m_ArrayBufferSize = 0;
		m_IndexBufferSize = 0;

		//Create a shape batch for each draw mode.
		m_ShapeBatches.emplace_back(GLDrawMode::LINES);
		m_ShapeBatches.emplace_back(GLDrawMode::TRIANGLES);
	}

	ShapeRenderer::~ShapeRenderer()
	{
		//Delete vertex arrays.
		glDeleteVertexArrays(1, &m_VAO);
	}

	//Starts batching primitives.
	void ShapeRenderer::StartBatching()
	{
		//Clear all vectors so they don't overflow memory.
		m_Shapes.clear();
		m_Vertices.clear();
		m_Indices.clear();

		m_VerticesToRender = 0;
		m_IndicesToRender = 0;

		m_ShapeBatches[GLDrawMode::LINES].indices = 0;
		m_ShapeBatches[GLDrawMode::TRIANGLES].indices = 0;
	}

	//Adds a shape to a list of shapes to be rendered at end draw.
	void ShapeRenderer::BatchShape(Shape* shape)
	{
		//Add the shape to the list of shapes that will be rendered.
		m_Shapes.push_back(shape);

		//Increase the number of vertices and indices that need to be rendered.
		m_VerticesToRender += shape->m_Vertices.size();
		m_IndicesToRender += shape->m_Indices.size();
	}

	//Sorts all of the shapes into batches and then draws them to the screen.
	void ShapeRenderer::EndBatching()
	{
		if (m_Shapes.size() == 0)
		{
			return;
		}

		//Sort all shapes by their draw mode.
		std::stable_sort(m_Shapes.begin(), m_Shapes.end(), CompareGLDrawMode);

		//Add all of the shapes vertices and indices into two main vectors that will be sent to the GPU.
		unsigned int currentDrawMode = 0;
		unsigned int currentVertice = 0;
		unsigned int currentIndice = 0;
		unsigned int numberOfVertices = 0;

		m_Vertices.resize(m_VerticesToRender);
		m_Indices.resize(m_IndicesToRender);

		for (unsigned int i = 0; i < m_Shapes.size(); i++)
		{
			//Add all vertices to the current batch.
			for (unsigned int c = 0; c < m_Shapes[i]->m_Vertices.size(); c++)
			{
				m_Vertices[currentVertice] = m_Shapes[i]->m_Vertices[c];
				currentVertice++;
			}

			for (unsigned int c = 0; c < m_Shapes[i]->m_Indices.size(); c++)
			{
				if (i > 0)
				{
					m_Indices[currentIndice] = m_Shapes[i]->m_Indices[c] + numberOfVertices;
				}

				else
				{
					m_Indices[currentIndice] = m_Shapes[i]->m_Indices[c];
				}

				currentIndice++;
			}

			numberOfVertices += m_Shapes[i]->m_Vertices.size();

			//Add indices to the current shape batch.
			if (currentDrawMode == m_Shapes[i]->GetGLDrawMode())
			{
				m_ShapeBatches[currentDrawMode].indices += m_Shapes[i]->m_Indices.size();
			}

			else
			{
				currentDrawMode++;
				m_ShapeBatches[currentDrawMode].indices += m_Shapes[i]->m_Indices.size();
			}
		}

		//Update the vertices and indices in the buffers.
		UpdateGPU();
		DrawShapeBatches();
	}

	//Updates the vertices and indices inside the vertex and indice buffer objects in the GPU.
	void ShapeRenderer::UpdateGPU()
	{
		//Bind vertex attribute object and index buffers.
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		//If the array buffer size or the indices buffer size is different from what is used to be, create and initialize new buffer space on the GPU.
		if (m_Vertices.size() * sizeof(PrimitiveVertex) != m_ArrayBufferSize || m_Indices.size() * sizeof(GLuint) != m_IndexBufferSize)
		{
			glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(PrimitiveVertex), m_Vertices.data(), GL_DYNAMIC_DRAW);
			m_ArrayBufferSize = m_Vertices.size() * sizeof(PrimitiveVertex);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(GLuint), m_Indices.data(), GL_DYNAMIC_DRAW);
			m_IndexBufferSize = m_Indices.size() * sizeof(GLuint);
		}

		//If the size of the array buffer and indices buffer is the same as what is used to be, update the vertices and indices already there.
		else
		{
			//Updates a subset of a buffer object's data store.
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_Vertices.size() * sizeof(PrimitiveVertex), m_Vertices.data());
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_Indices.size() * sizeof(GLuint), m_Indices.data());
		}

		//Unbind the vertex buffer object.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//Draws the shape batches to the screen with their selected draw mode.
	void ShapeRenderer::DrawShapeBatches()
	{
		//Bind the vertex array object.
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		//Use the primitive shader.
		m_Renderer->UseShader(Shaders::Shader_Primitive);
		m_Renderer->UpdateShaderUniforms();

		//Draw all the shape batches with their set draw mode.
		for (unsigned int i = 0; i < m_ShapeBatches.size(); i++)
		{
			//Only draw the batch if there are indices to draw.
			if (m_ShapeBatches[i].indices != 0)
			{
				unsigned int offSet = 0;
				switch (m_ShapeBatches[i].GLDrawMode)
				{
					case GLDrawMode::LINES:
					{
						glEnable(GL_LINE_SMOOTH);
						glDrawElements(GL_LINES, m_ShapeBatches[GLDrawMode::LINES].indices, GL_UNSIGNED_INT, 0);
						glDisable(GL_LINE_SMOOTH);
					}
					break;

					case GLDrawMode::TRIANGLES:
					{
						offSet += m_ShapeBatches[GLDrawMode::LINES].indices;
						glDrawElements(GL_TRIANGLES, m_ShapeBatches[GLDrawMode::TRIANGLES].indices, GL_UNSIGNED_INT, (void*)(offSet * sizeof(GLuint)));
					}
					break;
				}
			}
		}

		//Unbind VAO.
		glBindVertexArray(0);

		//Store the number of vertices and indices rendered this frame for performance tracking.
		m_VerticesRendering = m_Vertices.size();
		m_IndicesRendering = m_Indices.size();
	}

	//Compares the shapes draw mode.
	bool ShapeRenderer::CompareGLDrawMode(Shape * a, Shape* b)
	{
		return (a->GetGLDrawMode() < b->GetGLDrawMode());
	}

	bool ShapeRenderer::CompareDepth(Shape* a, Shape* b)
	{
		return (a->GetDepth() < b->GetDepth());
	}
}