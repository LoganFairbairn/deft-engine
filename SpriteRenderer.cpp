#include <algorithm>		//Included here for stable_sort.
#include "SpriteRenderer.h"

namespace Deft
{
	SpriteRenderer::SpriteRenderer(Renderer* renderer)
	{
		m_Renderer = renderer;

		//When the SpriteBatch object is initialized, an OpenGL array of vertices are created.
		CreateVertexArray();
	}

	SpriteRenderer::~SpriteRenderer()
	{
		glDeleteVertexArrays(1, &m_VAO);
	}

	//Adds a sprite to the vector of sprites (to be drawn in groups later).
	void SpriteRenderer::BatchSprite(Material material, Vec2 position, Vec2 size, float rotation, Vec2 originPoint, ColorRGBA8 color, float depth, Vec4 uvRectangle)
	{
		m_Sprites.emplace_back(material, position, size, rotation, originPoint, color, depth, uvRectangle);
	}

	//Begins sprite batching.
	//Sets the sorting type for sprites, clears all render batches in preparation for new ones, clears all Glyphs in preparation for new ones.
	void SpriteRenderer::StartBatching()
	{
		//Clear all vectors so they don't overflow memory.
		m_SpriteBatches.clear();
		m_Sprites.clear();
		m_Vertices.clear();
		m_Indices.clear();
	}

	//Ends sprite batching.
	//Creates sprite batches and then draws them to the screen.
	void SpriteRenderer::EndBatching()
	{
		//Set sprite pointers for each of the Sprite objects. Sorting pointers is more efficient than sorting objects.
		m_SpritePointers.resize(m_Sprites.size());
		for (unsigned int i = 0; i < m_Sprites.size(); i++)
		{
			m_SpritePointers[i] = &m_Sprites[i];
		}

		//Sort the sprites into groups based on material ID.
		SortSprites();

		//Creates render batches.
		CreateSpriteBatches();

		//Draws the sprite batches.
		DrawBatches();
	}

	//Creates new batches of vertices to be rendered.
	void SpriteRenderer::CreateSpriteBatches()
	{
		m_Vertices.resize(m_Sprites.size() * 4);	//4 vertices per sprite (to define a square).
		m_Indices.resize(m_Sprites.size() * 6);		//6 vertices per sprite.

		//If there are no sprites that need to be drawn, don't create any batches.
		if (m_Sprites.empty())
		{
			return;
		}

		//Store the vertices from the first sprite in the sprites ready to be drawn manually into the vertices for this sprite batch.
		unsigned int currentVertex = 0;
		m_SpriteBatches.emplace_back(4, m_SpritePointers[0]->GetTextureID(), m_SpritePointers[0]->GetShaderID());
		m_Vertices[currentVertex++] = m_SpritePointers[0]->m_Vertices[0];
		m_Vertices[currentVertex++] = m_SpritePointers[0]->m_Vertices[1];
		m_Vertices[currentVertex++] = m_SpritePointers[0]->m_Vertices[2];
		m_Vertices[currentVertex++] = m_SpritePointers[0]->m_Vertices[3];
		m_SpriteBatches.back().numberOfVertices = 4;

		unsigned int currentIndice = 0;
		m_Indices[currentIndice++] = 0;
		m_Indices[currentIndice++] = 1;
		m_Indices[currentIndice++] = 2;
		m_Indices[currentIndice++] = 0;
		m_Indices[currentIndice++] = 2;
		m_Indices[currentIndice++] = 3;

		//Cycle through the rest of the sprites and store their vertices and indices into the main list.
		for (unsigned int i = 1; i < m_Sprites.size(); i++)
		{
			//If the current material ID is different, make a new render batch.
			if (m_SpritePointers[i]->GetMaterialID() != m_SpritePointers[i - 1]->GetMaterialID())
			{
				m_SpriteBatches.emplace_back(4, m_SpritePointers[i]->GetTextureID(), m_SpritePointers[i]->GetShaderID());
			}

			else
			{
				m_SpriteBatches.back().numberOfVertices += 4;
			}

			//Set the vertices for this sprite in the render batch (and update the number of vertices in the render batch).
			m_Vertices[currentVertex++] = m_SpritePointers[i]->m_Vertices[0];
			m_Vertices[currentVertex++] = m_SpritePointers[i]->m_Vertices[1];
			m_Vertices[currentVertex++] = m_SpritePointers[i]->m_Vertices[2];
			m_Vertices[currentVertex++] = m_SpritePointers[i]->m_Vertices[3];

			//Set the indices for this sprite in the render batch.
			m_Indices[currentIndice++] = 0 + (4 * i);
			m_Indices[currentIndice++] = 1 + (4 * i);
			m_Indices[currentIndice++] = 2 + (4 * i);
			m_Indices[currentIndice++] = 0 + (4 * i);
			m_Indices[currentIndice++] = 2 + (4 * i);
			m_Indices[currentIndice++] = 3 + (4 * i);
		}

		//Update the vertices and Indices in the GPU buffer.
		//Bind vertex attribute object and index buffers.
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		//If the array buffer size or the indices buffer size is different from what is used to be, create and initialize new buffer space on the GPU.
		if (m_Vertices.size() * sizeof(Vertex) != m_ArrayBufferSize || m_Indices.size() * sizeof(GLuint) != m_IndexBufferSize)
		{
			glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_DYNAMIC_DRAW);
			m_ArrayBufferSize = m_Vertices.size() * sizeof(Vertex);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(GLuint), m_Indices.data(), GL_DYNAMIC_DRAW);
			m_IndexBufferSize = m_Indices.size() * sizeof(GLuint);
		}

		//If the size of the array buffer and indices buffer is the same as what is used t o be, update the vertices and indices already there.
		else
		{
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_Vertices.size() * sizeof(Vertex), m_Vertices.data());
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_Indices.size() * sizeof(GLuint), m_Indices.data());
		}

		//Unbind the vertex buffer object.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//Loops through all the render batches and renders them to the screen.
	//Call this function just after calling the End() function.
	void SpriteRenderer::DrawBatches()
	{
		//Bind the vertex array object and index buffer.
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		//Set the active texture.
		glActiveTexture(GL_TEXTURE0);

		//Loop through all the sprite batches and render them to the screen.
		unsigned int offSet = 0;
		for (unsigned int i = 0; i < m_SpriteBatches.size(); i++)
		{
			//Use the shader for the current render batch.
			m_Renderer->UseShader(m_SpriteBatches[i].shaderID);

			//Bind the texture for the current render batch.
			glBindTexture(GL_TEXTURE_2D, m_SpriteBatches[i].textureID);

			m_Renderer->TextureUniformRequiresUpdate(true);
			m_Renderer->UpdateShaderUniforms();

			//Draw the render batch using the OpenGL draw triangles method.
			GLuint indiceCount = (m_SpriteBatches[i].numberOfVertices / 4) * 6;
			glDrawElements(GL_TRIANGLES, indiceCount, GL_UNSIGNED_INT, (void*)(offSet * sizeof(GLuint)));

			offSet += indiceCount;
		}

		//Unbind VAO.
		glBindVertexArray(0);

		//Store the number of vertices and indices rendered this frame for performance tracking.
		m_VerticesRendering = m_Vertices.size();
		m_IndicesRendering = m_Indices.size();
	}

	//Creates an OpenGL Vertex Array Object.
	void SpriteRenderer::CreateVertexArray()
	{
		//Check to make sure there isn't a vertex array object already generated. If there is one already made, this will cause a memory leak.
		if (m_VAO == 0)
		{
			glGenVertexArrays(1, &m_VAO);
		}

		//Bind the vertex array object.
		glBindVertexArray(m_VAO);

		//Check that there isn't a vertex buffer objecter already generated too.
		if (m_VBO == 0)
		{
			glGenBuffers(1, &m_VBO);
		}

		//Generate a index buffer.
		glGenBuffers(1, &m_IndexBuffer);

		//Bind the vertex buffer object.
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		//Enable any required attribute arrays (position, color, uv).
		//Note: Enabling these vertex attribute arrays casuses overhead, avoid calling these as much as possible.
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//Define the vertex attributes.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		//Disables all the vertex attribute arrays, and unbinds the vertex buffer object.
		glBindVertexArray(0);
	}

	//Sorts the sprites into groups based on their material ID number.
	void SpriteRenderer::SortSprites()
	{
		//Stable sort ensures elements with the same value will be sorted in the original order.
		//Sort the sprites by their material.
		//std::stable_sort(m_SpritePointers.begin(), m_SpritePointers.end(), CompareMaterial);

		//Then sort the sprites by their depth.
		std::stable_sort(m_SpritePointers.begin(), m_SpritePointers.end(), CompareDepth);
	}

	bool SpriteRenderer::CompareMaterial(Sprite* a, Sprite* b)
	{
		return (a->GetMaterialID() < b->GetMaterialID());
	}

	bool SpriteRenderer::CompareDepth(Sprite* a, Sprite* b)
	{
		return (a->GetDepth() > b->GetDepth());
	}
}
