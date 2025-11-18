#include "pch.h"
#include "Entity.h"

namespace FL
{
	Entity::Entity(entt::entity e, Scene* scene)
		:m_entity(e), m_Scene(scene)
	{
	}

	Entity::~Entity()
	{

	}
}