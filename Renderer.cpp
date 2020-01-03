#include "Renderer.h"

namespace Deft
{
	//Initialize stuff needed for drawing to the screen (window, shaders, cameras).
	Renderer::Renderer()
	{
		//Set double buffering (used to reduce window flickering).
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		
		//Creates a window for the application.
		m_Window.CreateWindow("Deft Engine", DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, 0);

		//Sets the default window clear color.
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		//Initialize cameras.
		m_Camera.Initialize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
		m_UICamera.Initialize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
		m_UICamera.SetPosition(glm::vec2(DEFAULT_SCREEN_WIDTH * 0.5f, DEFAULT_SCREEN_HEIGHT * 0.5f));

		//Create optimized renderers for drawing sprites and shapes to the screen.
		m_GameSpriteRenderer = std::make_unique<SpriteRenderer>(this);
		m_UISpriteRenderer = std::make_unique<SpriteRenderer>(this);
		m_WindowSpriteRenderer = std::make_unique<SpriteRenderer>(this);

		m_UIShapeRenderer = std::make_unique<ShapeRenderer>(this);
		m_WindowShapeRenderer = std::make_unique<ShapeRenderer>(this);

		//Set the starting draw mode for the renderer.
		m_DrawMode = RendererDrawMode::DRAW_MODE_GAME;

		//Uniforms should update on the first frame.
		m_CameraUniformRequiresUpdate = true;
		m_TextureUniformRequiresUpdate = true;
	}

	//Updates the renderer.
	void Renderer::Update()
	{
		//Update cameras.
		m_Camera.Update();
		m_UICamera.Update();

		//Reset debug variables.
		m_UniqueTexturesRendering = 0;
		m_VerticesRendering = 0;
		m_IndicesRendering = 0;
	}

	//Uses the shader with the given ID number (ID is assigned when shader is loaded).
	void Renderer::UseShader(unsigned int shaderID)
	{
		if (m_CurrentShader != shaderID)
		{
			switch (shaderID)
			{
				case Shaders::Shader_Default:
				{
					Shader* shader = ResourceManager::GetShader(Shader_Default);
					shader->Use();
					m_CurrentShader = (unsigned int)Shaders::Shader_Default;

					m_CameraUniformRequiresUpdate = true;
					m_TextureUniformRequiresUpdate = true;
				}
				break;

				case Shaders::Shader_Primitive:
				{
					Shader* shader = ResourceManager::GetShader(Shader_Primitive);
					shader->Use();
					m_CurrentShader = (unsigned int)Shaders::Shader_Primitive;

					m_CameraUniformRequiresUpdate = true;
				}
				break;

				case Shaders::Shader_Red:
				{
					Shader* shader = ResourceManager::GetShader(Shader_Red);
					shader->Use();
					m_CurrentShader = (unsigned int)Shaders::Shader_Red;

					m_CameraUniformRequiresUpdate = true;
					m_TextureUniformRequiresUpdate = true;
				}
				break;
			}
		}
	}

	//UnUses the shader that's currently being used.
	void Renderer::UnUseShader()
	{
		Shader* shader = ResourceManager::GetShader(m_CurrentShader);
		shader->UnUse();
	}

	//Updates uniform variables for the shader currently in use based on the current renderer draw mode (if they need to be updated).
	void Renderer::UpdateShaderUniforms()
	{
		switch (m_CurrentShader)
		{
			case Shaders::Shader_Default:
			{
				Shader* shader = ResourceManager::GetShader(Shader_Default);

				UpdateProjectionMatrixUniform(shader);
				UpdateTextureUniform(shader);
			}
			break;

			case Shaders::Shader_Primitive:
			{
				Shader* shader = ResourceManager::GetShader(Shaders::Shader_Primitive);

				UpdateProjectionMatrixUniform(shader);
			}
			break;

			case Shaders::Shader_Red:
			{
				Shader* shader = ResourceManager::GetShader(Shader_Red);

				UpdateProjectionMatrixUniform(shader);
				UpdateTextureUniform(shader);
			}
			break;
		}
	}

	//Updates the texture uniform for the given shader if it's required.
	void Renderer::UpdateTextureUniform(Shader* shader)
	{
		if (m_TextureUniformRequiresUpdate == true)
		{
			GLint textureUniform = shader->GetUniformLocation("u_Sampler2D");
			glUniform1i(textureUniform, 0);

			m_TextureUniformRequiresUpdate = false;
		}

		m_TextureUniformRequiresUpdate++;
	}

	//Updates the projection matrix uniform for the given shader.
	void Renderer::UpdateProjectionMatrixUniform(Shader* shader)
	{
		if (m_CameraUniformRequiresUpdate == true)
		{
			switch (m_DrawMode)
			{
				case RendererDrawMode::DRAW_MODE_GAME:
				{
					glm::mat4 projectionMatrix = m_Camera.GetCameraMatrix();
					GLuint projectionMatrixUniform = shader->GetUniformLocation("projectionMatrix");
					glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, &projectionMatrix[0][0]);
				}

				break;

				case RendererDrawMode::DRAW_MODE_UI:
				{
					glm::mat4 projectionMatrix = m_UICamera.GetCameraMatrix();
					GLuint projectionMatrixUniform = shader->GetUniformLocation("projectionMatrix");
					glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, &projectionMatrix[0][0]);
				}
				break;

				case RendererDrawMode::DRAW_MODE_UI_WINDOWS:
				{

				}
				break;
			}

			m_CameraUniformRequiresUpdate = false;
		}
	}

	//Clears the whole screen with the set window color (call this before drawing).
	void Renderer::ClearScreen()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	//Swaps the window buffer (call this after drawing).
	void Renderer::SwapWindowBuffer()
	{
		m_Window.SwapWindowBuffer();
	}

	//Starts sprite and shape batching based on the set renderer draw mode.
	void Renderer::StartBatching(RendererDrawMode rendererDrawMode)
	{
		m_DrawMode = rendererDrawMode;

		switch (m_DrawMode)
		{
			case RendererDrawMode::DRAW_MODE_GAME:
			{
				m_GameSpriteRenderer->StartBatching();
			}
			break;

			case RendererDrawMode::DRAW_MODE_UI:
			{
				m_UISpriteRenderer->StartBatching();
				m_UIShapeRenderer->StartBatching();
			}
			break;

			case RendererDrawMode::DRAW_MODE_UI_WINDOWS:
			{
				m_WindowSpriteRenderer->StartBatching();
				m_WindowShapeRenderer->StartBatching();
			}
			break;
		}
	}

	//Organizes and draws sprites and shapes to the screen based on the set renderer draw mode.
	void Renderer::EndBatching()
	{
		switch (m_DrawMode)
		{
			case RendererDrawMode::DRAW_MODE_GAME:
			{
				m_GameSpriteRenderer->EndBatching();

				m_VerticesRendering += m_GameSpriteRenderer->GetVerticesRendering();
				m_IndicesRendering += m_GameSpriteRenderer->GetIndicesRendering();
			}
			break;

			case RendererDrawMode::DRAW_MODE_UI:
			{
				m_UIShapeRenderer->EndBatching();		//Always end shape batching first.
				m_UISpriteRenderer->EndBatching();

				m_VerticesRendering += m_UISpriteRenderer->GetVerticesRendering();
				m_VerticesRendering += m_UIShapeRenderer->GetVerticesRendering();

				m_IndicesRendering += m_UISpriteRenderer->GetIndicesRendering();
				m_IndicesRendering += m_UIShapeRenderer->GetIndicesRendering();
			}
			break;

			case RendererDrawMode::DRAW_MODE_UI_WINDOWS:
			{
				m_WindowShapeRenderer->EndBatching();	//Always end shape batching first.
				m_WindowSpriteRenderer->EndBatching();

				m_VerticesRendering += m_WindowSpriteRenderer->GetVerticesRendering();
				m_VerticesRendering += m_WindowShapeRenderer->GetVerticesRendering();

				m_IndicesRendering += m_WindowSpriteRenderer->GetIndicesRendering();
				m_IndicesRendering += m_WindowShapeRenderer->GetIndicesRendering();
			}
			break;
		}
	}

	//Adds a sprite to a vector to be drawn with sprite batching.
	void Renderer::DrawSprite(Material material, Vec2 position)
	{
		//Only draw the sprite if it's in view.
		if (m_Camera.IsBoxInView(position.x, position.y, (float)material.texture.width, (float)material.texture.height))
		{
			//Set default valeus for drawing.
			Vec2 size = Vec2(material.texture.width, material.texture.height);
			float rotation = 0.0f;
			Vec2 originPoint = Vec2(0.0f, 0.0f);
			ColorRGBA8 color = ColorRGBA8(255, 255, 255, 255);
			float depth = 0.5f;
			Vec4 uvRectangle = Vec4(0.0f, 0.0f, 1.0f, 1.0f);

			switch (m_DrawMode)
			{
				case RendererDrawMode::DRAW_MODE_GAME:
				{
					m_GameSpriteRenderer->BatchSprite(material, position, size, rotation, originPoint, color, depth, uvRectangle);
				}
				break;

				case RendererDrawMode::DRAW_MODE_UI:
				{
					m_UISpriteRenderer->BatchSprite(material, position, size, rotation, originPoint, color, depth, uvRectangle);
				}
				break;

				case RendererDrawMode::DRAW_MODE_UI_WINDOWS:
				{
					m_WindowSpriteRenderer->BatchSprite(material, position, size, rotation, originPoint, color, depth, uvRectangle);
				}
				break;
			}
		}
	}

	//Adds a sprite to a vector to be drawn with sprite batching.
	void Renderer::DrawSprite(Material material, Vec2 position, Vec2 scale, Vec2 pivot, float depth)
	{
		//Only draw the sprite if it's in view.
		if (m_Camera.IsBoxInView(position.x, position.y, (float)material.texture.width, (float)material.texture.height))
		{
			//Set default valeus for drawing.
			Vec2 size;
			size.x = material.texture.width * scale.x;
			size.y = material.texture.height * scale.y;

			ColorRGBA8 color = ColorRGBA8(255, 255, 255, 255);
			Vec4 uvRectangle = Vec4(0.0f, 0.0f, 1.0f, 1.0f);

			switch (m_DrawMode)
			{
				case RendererDrawMode::DRAW_MODE_GAME:
				{
					m_GameSpriteRenderer->BatchSprite(material, position, size, 0.0f, pivot, color, depth, uvRectangle);
				}
				break;

				case RendererDrawMode::DRAW_MODE_UI:
				{
					m_UISpriteRenderer->BatchSprite(material, position, size, 0.0f, pivot, color, depth, uvRectangle);
				}
				break;

				case RendererDrawMode::DRAW_MODE_UI_WINDOWS:
				{
					m_WindowSpriteRenderer->BatchSprite(material, position, size, 0.0f, pivot, color, depth, uvRectangle);
				}
				break;
			}
		}
	}

	//Adds a sprite to a vector to be drawn with sprite batching based on the set renderer draw mode.
	void Renderer::DrawSprite(Material material, Vec2 position, Vec2 scale, Vec2 pivot, ColorRGBA8 color, float depth)
	{
		//Only draw the sprite if it's in view.
		if (m_Camera.IsBoxInView(position.x, position.y, (float)material.texture.width, (float)material.texture.height))
		{
			//Set default valeus for drawing.
			Vec2 size;
			size.x = material.texture.width * scale.x;
			size.y = material.texture.height * scale.y;

			Vec4 uvRectangle = Vec4(0.0f, 0.0f, 1.0f, 1.0f);

			switch (m_DrawMode)
			{
				case RendererDrawMode::DRAW_MODE_GAME:
				{
					m_GameSpriteRenderer->BatchSprite(material, position, size, 0.0f, pivot, color, depth, uvRectangle);
				}
				break;

				case RendererDrawMode::DRAW_MODE_UI:
				{
					m_UISpriteRenderer->BatchSprite(material, position, size, 0.0f, pivot, color, depth, uvRectangle);
				}
				break;

				case RendererDrawMode::DRAW_MODE_UI_WINDOWS:
				{
					m_WindowSpriteRenderer->BatchSprite(material, position, size, 0.0f, pivot, color, depth, uvRectangle);
				}
				break;
			}
		}
	}

	//Adds a sprite to a vector to be drawn with sprite batching based on the set renderer draw mode.
	void Renderer::DrawSprite(Material material, Vec2 position, float rotation, Vec2 pivot, ColorRGBA8 color, float depth)
	{
		//Only draw the sprite if it's in view.
		if (m_Camera.IsBoxInView(position.x, position.y, (float)material.texture.width, (float)material.texture.height))
		{
			Vec4 uvRectangle = Vec4(0.0f, 0.0f, 1.0f, 1.0f);
			Vec2 size = Vec2(material.texture.width, material.texture.height);

			switch (m_DrawMode)
			{
				case RendererDrawMode::DRAW_MODE_GAME:
				{
					m_GameSpriteRenderer->BatchSprite(material, position, size, rotation, pivot, color, depth, uvRectangle);
				}
				break;

				case RendererDrawMode::DRAW_MODE_UI:
				{
					m_UISpriteRenderer->BatchSprite(material, position, size, rotation, pivot, color, depth, uvRectangle);
				}
				break;

				case RendererDrawMode::DRAW_MODE_UI_WINDOWS:
				{
					m_WindowSpriteRenderer->BatchSprite(material, position, size, rotation, pivot, color, depth, uvRectangle);
				}
				break;
			}
		}
	}

	//Adds a sprite to a vector to be drawn with sprite batching based on the set renderer draw mode.
	void Renderer::DrawSprite(Material material, Vec2 position, float rotation, Vec2 pivot, ColorRGBA8 color, float depth, Vec4 uvRectangle)
	{
		//Only draw the sprite if it's in view.
		if (m_Camera.IsBoxInView(position.x, position.y, (float)material.texture.width, (float)material.texture.height))
		{
			Vec2 size = Vec2(material.texture.width, material.texture.height);

			switch (m_DrawMode)
			{
				case RendererDrawMode::DRAW_MODE_GAME:
				{
					m_GameSpriteRenderer->BatchSprite(material, position, size, rotation, pivot, color, depth, uvRectangle);
				}
				break;

				case RendererDrawMode::DRAW_MODE_UI:
				{
					m_UISpriteRenderer->BatchSprite(material, position, size, rotation, pivot, color, depth, uvRectangle);
				}
				break;

				case RendererDrawMode::DRAW_MODE_UI_WINDOWS:
				{
					m_WindowSpriteRenderer->BatchSprite(material, position, size, rotation, pivot, color, depth, uvRectangle);
				}
				break;
			}
		}
	}

	//Adds a sprite to a vector to be drawn with sprite batching based on the set renderer draw mode.
	void Renderer::DrawSprite(Material material, Vec2 position, Vec2 size, float rotation, Vec2 pivot, ColorRGBA8 color, float depth, Vec4 uvRectangle)
	{
		//Only draw the sprite if it's in view.
		if (m_Camera.IsBoxInView(position.x, position.y, (float)material.texture.width, (float)material.texture.height))
		{
			switch (m_DrawMode)
			{
				case RendererDrawMode::DRAW_MODE_GAME:
				{
					m_GameSpriteRenderer->BatchSprite(material, position, size, rotation, pivot, color, depth, uvRectangle);
				}
				break;

				case RendererDrawMode::DRAW_MODE_UI:
				{
					m_UISpriteRenderer->BatchSprite(material, position, size, rotation, pivot, color, depth, uvRectangle);
				}
				break;

				case RendererDrawMode::DRAW_MODE_UI_WINDOWS:
				{
					m_WindowSpriteRenderer->BatchSprite(material, position, size, rotation, pivot, color, depth, uvRectangle);
				}
				break;
			}
		}
	}

	//Draws text to the screen using sprite batching.
	void Renderer::DrawText(SpriteFont* font, const std::string& text, Vec2 position)
	{
		Vec2 scale = Vec2(1.0f, 1.0f);
		ColorRGBA8 color = ColorRGBA8(255, 255, 255, 255);
		font->DrawText(this, text.c_str(), position, scale, -0.6f, color, TextAlignment::LEFT);
	}

	//Draws text to the screen using sprite batching.
	void Renderer::DrawText(SpriteFont* font, const std::string& text, Vec2 position, ColorRGBA8 color)
	{
		font->DrawText(this, text.c_str(), position, Vec2(1.0f, 1.0f), 0.0f, color, TextAlignment::LEFT);
	}

	//Draws text to the screen using sprite batching.
	void Renderer::DrawText(SpriteFont* font, const std::string& text, Vec2 position, ColorRGBA8 color, TextAlignment alignment)
	{
		font->DrawText(this, text.c_str(), position, Vec2(1.0f, 1.0f), 0.0f, color, alignment);
	}

	//Draws text to the screen using sprite batching.
	void Renderer::DrawText(SpriteFont* font, const std::string& text, Vec2 position, ColorRGBA8 color, TextAlignment alignment, Vec2 scale, float depth)
	{
		font->DrawText(this, text.c_str(), position, scale, depth, color, alignment);
	}

	//Adds a shape to a vector of shapes that will be rendered later.
	void Renderer::DrawShape(Shape* shape)
	{
		//Batch the shape based on the current draw mode.
		switch (m_DrawMode)
		{
			case RendererDrawMode::DRAW_MODE_UI:
			{
				m_UIShapeRenderer->BatchShape(shape);
			}
			break;

			case RendererDrawMode::DRAW_MODE_UI_WINDOWS:
			{
				m_WindowShapeRenderer->BatchShape(shape);
			}
			break;
		}
	}

	//Defines a rectangle and adds it to a vector of shapes that will be rendered later.
	void Renderer::DrawRectangle(Vec2 position, Vec2 size, ColorRGBA8 color, float depth)
	{
		//Only draw the shape if it's in view.
		if (m_Camera.IsBoxInView(position.x, position.y, size.x, size.y))
		{
			Shape* shape = new Shape();

			//Set vertices.
			shape->m_Vertices.resize(4);
			shape->m_Vertices[0].SetPosition(position.x, position.y);
			shape->m_Vertices[1].SetPosition(position.x, position.y + size.y);
			shape->m_Vertices[2].SetPosition(position.x + size.x, position.y + size.y);
			shape->m_Vertices[3].SetPosition(position.x + size.x, position.y);

			//Set indices.
			shape->m_Indices.resize(6);
			shape->m_Indices[0] = 0;
			shape->m_Indices[1] = 1;
			shape->m_Indices[2] = 2;
			shape->m_Indices[3] = 0;
			shape->m_Indices[4] = 2;
			shape->m_Indices[5] = 3;

			shape->SetColor(color);
			shape->SetDepth(depth);
			shape->SetGLDrawMode(GLDrawMode::TRIANGLES);

			//Batch the shape based on the current draw mode.
			switch (m_DrawMode)
			{
				case RendererDrawMode::DRAW_MODE_UI:
				{
					m_UIShapeRenderer->BatchShape(shape);
				}
				break;

				case RendererDrawMode::DRAW_MODE_UI_WINDOWS:
				{
					m_WindowShapeRenderer->BatchShape(shape);
				}
				break;
			}
		}
	}

	//Defines a rectangle outline and adds it to a vector of shapes that will be rendered later.
	void Renderer::DrawRectangleOutline(Vec2 position, Vec2 size, ColorRGBA8 color, float depth)
	{
		//Only draw the shape if it's in view.
		if (m_Camera.IsBoxInView(position.x, position.y, size.x, size.y))
		{
			Shape* shape = new Shape();

			//Set vertices.
			shape->m_Vertices.resize(4);
			shape->m_Vertices[0].SetPosition(position.x, position.y);
			shape->m_Vertices[1].SetPosition(position.x, position.y + size.y);
			shape->m_Vertices[2].SetPosition(position.x + size.x, position.y + size.y);
			shape->m_Vertices[3].SetPosition(position.x + size.x, position.y);

			//Set indices.
			shape->m_Indices.resize(8);
			shape->m_Indices[0] = 0;
			shape->m_Indices[1] = 1;
			shape->m_Indices[2] = 1;
			shape->m_Indices[3] = 2;
			shape->m_Indices[4] = 2;
			shape->m_Indices[5] = 3;
			shape->m_Indices[6] = 3;
			shape->m_Indices[7] = 0;

			shape->SetColor(color);
			shape->SetDepth(depth);
			shape->SetGLDrawMode(GLDrawMode::LINES);

			//Batch the shape based on the current draw mode.
			switch (m_DrawMode)
			{
				case RendererDrawMode::DRAW_MODE_UI:
				{
					m_UIShapeRenderer->BatchShape(shape);
				}
				break;

				case RendererDrawMode::DRAW_MODE_UI_WINDOWS:
				{
					m_WindowShapeRenderer->BatchShape(shape);
				}
				break;
			}
		}
	}

	//Draws an info box (and keeps the box on screen).
	void Renderer::DrawInfoBox(Vec2 position, float offSet, const std::string& text, SpriteFont* font)
	{
		float padding = 10.0f;
		float textWidth = font->MeasureText(text.c_str()).x;
		float textHeight = font->MeasureText(text.c_str()).y;
		float boxWidth = textWidth + (padding * 2);
		float boxHeight = textHeight + (padding * 2);

		Vec2 boxPosition;
		boxPosition.x = position.x + offSet;
		boxPosition.y = position.y - (boxHeight * 0.5);

		//Keep the info box on screen.
		if (boxPosition.x < 0.0f)
		{
			boxPosition.x = 0;
		}

		else if (boxPosition.x + offSet + boxWidth > GetCameraWidth())
		{
			boxPosition.x = position.x - offSet - boxWidth;
		}

		if (boxPosition.y + boxHeight > GetCameraHeight())
		{
			boxPosition.y = GetCameraHeight() - boxHeight;
		}

		else if(boxPosition.y < 0.0f)
		{
			boxPosition.y = 0.0f;
		}

		DrawRectangleOutline(boxPosition, Vec2(boxWidth, boxHeight), ColorRGBA8(255, 255, 255, 255), -0.998f);
		DrawRectangle(boxPosition, Vec2(boxWidth, boxHeight), ColorRGBA8(0, 0, 0, 150), -0.99f);
		DrawText(font, text, Vec2(boxPosition.x + padding, boxPosition.y + padding), ColorRGBA8(255, 255, 255, 255), TextAlignment::LEFT, Vec2(1.0f, 1.0f), -0.999f);
	}

	//Transfers screen coordinates to world coordinates.
	Vec2 Renderer::ScreenToWorldCoordinates(Vec2 position)
	{
		glm::vec2 worldCoordinates = m_Camera.ScreenToWorldCoords(glm::vec2(position.x, position.y));
		return Vec2(worldCoordinates.x, worldCoordinates.y);
	}

	//Sets the clear color for the screen.
	void Renderer::SetClearColor(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
	}
}
