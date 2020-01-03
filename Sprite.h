#pragma once
#include <glm\glm.hpp>
#include <vector>
#include "Texture.h"
#include "Vertex.h"
#include "Vector.h"
#include "Material.h"

namespace Deft
{
	//A sprite requires sprite batching to be drawn to the screen.
	class Sprite
	{
	public:
		Sprite();
		Sprite(Material material, Vec2 position, Vec2 size, float rotation, Vec2 originPoint, ColorRGBA8 color, float depth, Vec4 uvRectangle);

		//Setters.
		void SetMaterial(Material material) { m_Material = material; }
		void SetTransform(Vec2 position, Vec2 size, Vec2 originPoint);
		void SetUVRectangle(Vec4 uvRectangle);
		void SetColor(ColorRGBA8 color);
		void SetRotation(float rotation);
		void SetDepth(float depth);

		//Getters.
		unsigned int GetMaterialID() { return m_Material.materialID; }
		GLuint GetTextureID() { return m_Material.texture.id; }
		unsigned int GetShaderID() { return m_Material.shaderID; }
		float GetDepth();

		//Vertices to define the sprite.
		std::vector<Vertex> m_Vertices;

	private: 
		Vec2 RotatePoint(float x, float y, float angle);

		Material m_Material;
	};
}