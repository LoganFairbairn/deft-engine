#include "EntityManager.h"

namespace Deft
{
	//Create a pointer to the entity and add it to the list of entities.
	Entity* EntityManager::AddEntity(const std::string& entityName)
	{
		//Create an entity and give it an ID number.
		m_Entities.push_back(std::make_unique<Entity>(entityName));
		m_Entities.back()->SetID(m_Entities.size());

		return m_Entities.back().get();
	}

	//Draws all visible entities to the window.
	void EntityManager::DrawEntities(Renderer* renderer)
	{
		for (unsigned int i = 0; i < m_Entities.size(); i++)
		{
			MaterialComponent* materialComponent = m_Entities[i]->GetComponent<MaterialComponent>(ComponentType::MaterialComponent);
			TransformComponent* transformComponent = m_Entities[i]->GetComponent<TransformComponent>(ComponentType::TransformComponent);

			if (materialComponent != nullptr)
			{
				if (materialComponent->GetVisible() == true && transformComponent != nullptr)
				{
					Vec2 position = transformComponent->GetPosition();
					Vec2 size = Vec2(materialComponent->GetMaterial().texture.width, materialComponent->GetMaterial().texture.height);
					Vec2 scale = transformComponent->GetScale();
					float rotation = transformComponent->GetRotation();

					Material material = materialComponent->GetMaterial();
					ColorRGBA8 color = materialComponent->GetColor();
					Vec2 originPoint = materialComponent->GetOriginPoint();
					float depth = materialComponent->GetDepth();

					Vec4 uvRectangle = Vec4(0.0f, 0.0f, 1.0f, 1.0f);

					//Draw the sprite for the entity.
					renderer->DrawSprite(material,
						position,
						size,
						rotation,
						originPoint,
						color,
						depth,
						uvRectangle);
				}
			}
		}
	}

	//Note: Only call this during loading screens.
	//Removes all of the entities from the entity list and unloads all unused assets.
	void EntityManager::ClearEntities()
	{

	}
}