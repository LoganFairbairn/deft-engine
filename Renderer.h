#pragma once
#include <SDL\SDL_ttf.h>			//This allow true type font functionality to be used.
#include <memory>					//Included here for 
#include <algorithm>				//Included here for stable_sort.

#include "FatalErrors.h"			//Included for custom debug errors.
#include "RendererDrawMode.h"		//Available draw modes for the renderer.
#include "Camera2D.h"				//2D camera for calculating world view projection matrices.
#include "Window.h"					//Window the game will be rendered to.
#include "Shaders.h"				//Gives access to an enum list of available shader programs.
#include "Shader.h"					//Allows use of shader programs.

#include "SpriteRenderer.h"			//Allows sprites to be drawn in batches for a performance boost.
#include "SpriteFont.h"				//Allows text to be drawn as sprites for a performance boost when used with sprite batching.

#include "ShapeRenderer.h"			//Allows primitive shapes to be drawn in batches for a performance boost.
#include "Shape.h"

#include "TextAlignment.h"			//Available text alignments.
#include "Material.h"				//Allows use of materials.

#include "ResourceManager.h"		//Gives access to the game's resource caches.

namespace Deft
{
	class Shader;
	class ShapeRenderer;
	class SpriteRenderer;
	class SpriteFont;
	class ResourceManager;

	//This class provides simple optimized drawing functions that hide low level draw code (OpenGL, Vulkan, DirectX).
	class Renderer
	{
		const int DEFAULT_SCREEN_WIDTH = 1280;
		const int DEFAULT_SCREEN_HEIGHT = 720;

	public:
		Renderer();
		~Renderer() {};

		void Update();

		//Shader functions.
		void UseShader(unsigned int shaderID);
		void UnUseShader();
		void UpdateShaderUniforms();
		void UpdateTextureUniform(Shader* shader);
		void UpdateProjectionMatrixUniform(Shader* shader);
		void CameraUniformRequiresUpdate(bool requiresUpdate) { m_CameraUniformRequiresUpdate = true; }
		void TextureUniformRequiresUpdate(bool requiresUpdate) { m_TextureUniformRequiresUpdate = true; }

		//Drawing Functions.
		void ClearScreen();
		void SwapWindowBuffer();
		void StartBatching(RendererDrawMode rendererDrawMode);
		void EndBatching();

		//Sprite drawing functions (using sprite batching).
		void DrawSprite(Material material, Vec2 position);
		void DrawSprite(Material material, Vec2 position, Vec2 scale, Vec2 pivot, float depth);
		void DrawSprite(Material material, Vec2 position, Vec2 scale, Vec2 pivot, ColorRGBA8 color, float depth);
		void DrawSprite(Material material, Vec2 position, float rotation, Vec2 pivot, ColorRGBA8 color, float depth);
		void DrawSprite(Material material, Vec2 position, float rotation, Vec2 pivot, ColorRGBA8 color, float depth, Vec4 uvRectangle);
		void DrawSprite(Material material, Vec2 position, Vec2 size, float rotation, Vec2 pivot, ColorRGBA8 color, float depth, Vec4 uvRectangle);

		//Text drawing functions.
		void DrawText(SpriteFont* font, const std::string& text, Vec2 position);
		void DrawText(SpriteFont* font, const std::string& text, Vec2 position, ColorRGBA8 color);
		void DrawText(SpriteFont* font, const std::string& text, Vec2 position, ColorRGBA8 color, TextAlignment alignment);
		void DrawText(SpriteFont* font, const std::string& text, Vec2 position, ColorRGBA8 color, TextAlignment alignment, Vec2 scale, float depth);

		//Shape drawing functions.
		void DrawShape(Shape* shape);
		void DrawRectangle(Vec2 position, Vec2 size, ColorRGBA8 color, float depth);
		void DrawRectangleOutline(Vec2 position, Vec2 size, ColorRGBA8 color, float depth);

		//Info Boxes
		void DrawInfoBox(Vec2 position, float offSet, const std::string& text, SpriteFont* font);

		//Camera to world.
		Vec2 ScreenToWorldCoordinates(Vec2 position);

		//Setters.
		void SetDrawMode(RendererDrawMode drawMode) { m_DrawMode = drawMode; }
		void SetClearColor(float r, float g, float b);

		//Getters.
		unsigned int GetCameraWidth() { return m_Camera.GetCameraWidth(); }
		unsigned int GetCameraHeight() { return m_Camera.GetCameraHeight(); }
		
		unsigned int GetCurrentShader() { return m_CurrentShader; }

		//Debug getters.
		unsigned int GetUniqueTexturesRendering() { return m_UniqueTexturesRendering; }
		unsigned int GetVerticesRendering() { return m_VerticesRendering; }
		unsigned int GetIndicesRendering() { return m_IndicesRendering; }

	private:
		Window m_Window;				//Game window.
		RendererDrawMode m_DrawMode;	//Draw mode that determines which camera and which sprite / shape renderer to use.
		Camera2D m_Camera;				//Camera for regular drawing.
		Camera2D m_UICamera;			//Camera for drawing user interface.

		//These sprite and shape renderers allows massive performance boosts by grouping sprites or shapes together before drawing.
		//There are multiple shape and sprite renderers for each main draw call for the game. This is required to draw in the correct order.
		std::unique_ptr<SpriteRenderer> m_GameSpriteRenderer;
		std::unique_ptr<SpriteRenderer> m_UISpriteRenderer;
		std::unique_ptr<SpriteRenderer> m_WindowSpriteRenderer;

		std::unique_ptr<ShapeRenderer> m_UIShapeRenderer;
		std::unique_ptr<ShapeRenderer> m_WindowShapeRenderer;
		
		//TODO: Storing shaders here when they are loaded might make setting shaders faster, since you could skip the look-up in the Resource Manager.
		//Variables that keep track of what requires updating, to increase performance.
		unsigned int m_CurrentShader;
		bool m_CameraUniformRequiresUpdate;
		bool m_TextureUniformRequiresUpdate;

		//Performance tracking variables.
		unsigned int m_UniqueTexturesRendering = 0;
		unsigned int m_VerticesRendering = 0;
		unsigned int m_IndicesRendering = 0;
	};
}

