#pragma once
#include <memory>
#include <vector>
#include "Entity.h"
#include "Renderer.h"

#include "TransformComponent.h"
#include "MaterialComponent.h"

namespace Deft
{
	class EntityManager
	{
	public:
		EntityManager() {};
		~EntityManager() {};

		void DrawEntities(Renderer* renderer);

		Entity* AddEntity(const std::string& entityName);
		void ClearEntities();

	private:
		std::vector<std::unique_ptr<Entity>> m_Entities;
	};
}