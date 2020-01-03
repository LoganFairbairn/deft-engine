#pragma once
#include <GL/glew.h>
#include <glm\glm.hpp>
#include <vector>

#include "Vertex.h"
#include "Sprite.h"
#include "Renderer.h"

namespace Deft
{
	class Renderer;

	//This is a batch of sprites sorted by their material ID.
	class SpriteBatch
	{
	public:
		SpriteBatch(GLuint NumberOfVertices, GLuint TextureID, unsigned int ShaderID) : numberOfVertices(NumberOfVertices), textureID(TextureID), shaderID(ShaderID) {}
		
		GLuint numberOfVertices;		//Number of vertices in the render batch.
		GLuint textureID;				//ID of the texture for the batch.
		unsigned int shaderID;			//ID of the shader that will be used to draw this batch of sprites.
	};

	//Renders sprites in batches based on their material before drawing them to the screen, this provides a massiver performance boost.
	class SpriteRenderer
	{
	public:
		SpriteRenderer(Renderer* renderer);
		~SpriteRenderer();

		void StartBatching();
		void BatchSprite(Material material, Vec2 position, Vec2 size, float rotation, Vec2 originPoint, ColorRGBA8 color, float depth, Vec4 uvRectangle);
		void EndBatching();

		//Getters.
		unsigned int GetVerticesRendering() { return m_VerticesRendering; };
		unsigned int GetIndicesRendering() { return m_IndicesRendering; };

	private:
		void CreateSpriteBatches();
		void DrawBatches();
		void CreateVertexArray();
		void SortSprites();

		static bool CompareMaterial(Sprite* a, Sprite* b);
		static bool CompareDepth(Sprite* a, Sprite* b);

		Renderer* m_Renderer;	//Pointer to the renderer.

		GLuint m_VBO = 0;							//This is the OpenGL buffer object ID.
		GLuint m_VAO = 0;							//This is the OpenGL array object ID. 
		GLuint m_IndexBuffer;						//This is the OpenGL index buffer ID.
		GLuint m_ArrayBufferSize;					//Size of the array buffer sent to the GPU.
		GLuint m_IndexBufferSize;					//Size of the indice buffer sent to the GPU.

		std::vector<GLuint> m_Indices;
		std::vector<Vertex> m_Vertices;

		std::vector<Sprite> m_Sprites;				//These are the actual Sprite objects.
		std::vector<Sprite*> m_SpritePointers;		//This is a list of Sprite pointers used for efficient Sprite sorting.
		std::vector<SpriteBatch> m_SpriteBatches;	//This is a list of sprites sorted into batches that will be rendered to the screen.

		//Performance tracking variables.
		unsigned int m_VerticesRendering = 0;
		unsigned int m_IndicesRendering = 0;
	};
}
