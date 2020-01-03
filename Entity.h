#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Component.h"
#include "ComponentType.h" 

namespace Deft
{
	class Entity
	{
	public:
		Entity(const std::string& entityName) { m_Name = entityName; };
		~Entity() {};

		//Adds a component of the given type to this entity.
		template<class T>
		T* AddComponent(ComponentType componentType)
		{
			m_Components.push_back(std::make_unique<T>());

			//Set the components type and entity after you create it.
			Component* result = m_Components.back().get();
			result->SetType(componentType);
			result->SetEntity(this);

			return (T*)result;
		}

		//Gets a component attatched to this entity if one exists.
		template<class T>
		T* GetComponent(ComponentType component)
		{
			for (unsigned int i = 0; i < m_Components.size(); i++)
			{
				if (m_Components[i].get()->GetType() == component)
				{
					return (T*)m_Components[i].get();
				}
			}

			return nullptr;
		}

		//Setters.
		void SetID(unsigned int entityID) { m_ID = entityID; }
		void SetActive(bool active) { m_Active = active; };

		//Getters.
		unsigned int GetID() { return m_ID; }
		bool GetActive() { return m_Active; }

	private:
		bool m_Active = true;		//Only update active entities.
		unsigned int m_ID;
		std::string m_Name;
		std::vector<std::unique_ptr<Component>> m_Components;
	};
}