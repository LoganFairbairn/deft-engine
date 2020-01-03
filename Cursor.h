#pragma once
#include "Vector.h"
#include "Material.h"

namespace Deft
{
	class Cursor
	{
	public:
		Cursor() {};
		~Cursor() {};

		void SetPosition(Vec2 position) { m_Position = position; }
		void SetPivot(Vec2 pivot) { m_Pivot = pivot; }
		void SetMaterial(Material material) { m_Material = material; }

		Vec2 GetPosition() { return m_Position; }
		Vec2 GetPivot() { return m_Pivot; }
		Material GetMaterial() { return m_Material; }

	private:
		Vec2 m_Position;
		Vec2 m_Pivot;
		Material m_Material;
	};
}