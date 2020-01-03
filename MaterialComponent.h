#pragma once
#include "Component.h"
#include "Texture.h"
#include "Vertex.h"
#include "Shader.h"

namespace Deft
{
	class MaterialComponent : public Component
	{
	public:
		//Set default values when created.
		MaterialComponent() 
		{
			m_Material.materialID = 0;
			m_Material.shaderID = 0;
			m_Material.texture.id = 0;
			m_Material.texture.width = 0;
			m_Material.texture.height = 0;

			m_Color = ColorRGBA8(255, 255, 255, 255);
			m_Depth = 0.5f;
			m_Visible = true;
		}

		//Setters.
		void SetMaterial(Material material) { m_Material = material; }
		void SetColor(ColorRGBA8 color) { m_Color = color; }
		void SetOriginPoint(Vec2 originPoint) { m_OriginPoint = originPoint; }
		void SetDepth(float depth) { m_Depth = depth; }
		void SetVisible(bool visible) { m_Visible = visible; }

		//Getters.
		Material GetMaterial() { return m_Material; }
		ColorRGBA8 GetColor() { return m_Color; }
		Vec2 GetOriginPoint() { return m_OriginPoint; }
		float GetDepth() { return m_Depth; }
		bool GetVisible() { return m_Visible; }

	private:
		Material m_Material;
		ColorRGBA8 m_Color;
		Vec2 m_OriginPoint;
		float m_Depth;
		bool m_Visible;
	};
}