#pragma once
#include "Component.h"
#include "Vector.h"

namespace Deft
{
	class TransformComponent : public Component
	{
	public:
		TransformComponent() 
		{
			m_Position = Vec2(0.0f, 0.0f);
			m_Scale = Vec2(1.0f, 1.0f);
			m_Rotation = 0.0f;
		}

		//Setters.
		void SetPosition(Vec2 position) { m_Position = position; }
		void SetRotation(float angle) { m_Rotation = angle; }
		void SetScale(Vec2 scale) { m_Scale = scale; }

		//Getters.
		Vec2 GetPosition() { return m_Position; }
		float GetRotation() { return m_Rotation; }
		Vec2 GetScale() { return m_Scale; }

	private:
		Vec2 m_Position = Vec2(0.0f, 0.0f);
		float m_Rotation;
		Vec2 m_Scale = Vec2(0.0f, 0.0f);
	};
}