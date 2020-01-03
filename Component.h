#pragma once
#include <string>
#include "ComponentType.h"
#include "Entity.h"

namespace Deft
{
	class Entity;

	class Component
	{
	public:
		Component() {};
		~Component() {};

		void SetType(ComponentType componentType) { m_Type = componentType; }
		void SetEntity(Entity* entity) { m_Entity = entity; }

		ComponentType GetType() { return m_Type; }
		Entity* GetEntity() { return m_Entity; }

	private:
		bool m_Enabled;					//If the component isn't enabled, the System won't do anything with it.
		ComponentType m_Type;
		Entity* m_Entity;
	};
}