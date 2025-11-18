#pragma once
#include "Scene.h"

namespace FL
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity e, Scene* scene);
		~Entity();

		template<typename Component, typename ...Args>
		void AddComponent(Args &&... args)
		{
			m_Scene->m_Registry.emplace<Component>(m_entity,std::forward<Args>(args)...);
		}

		template<typename Component>
		void RemoveComponent()
		{
			if (this->HasComponent<Component>())
			{
				m_Scene->m_Registry.remove<Component>(m_entity);
			}
		}

		template<typename Component>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<Component>(m_entity);
		}

		template<typename Component>
		Component& GetComponent()
		{
			return m_Scene->m_Registry.get<Component>(m_entity);
		}

		template<typename Component>
		Component* TryGetComponent()
		{
			return m_Scene->m_Registry.try_get<Component>(m_entity);
		}

		bool operator==(Entity& entity)
		{
			return m_entity == entity.m_entity;
		}
		operator bool() const 
		{
			return m_entity != entt::null; 
		}
	private:
		entt::entity m_entity = { entt::null };
		Scene* m_Scene = nullptr;

		friend class Scene;
	};
};

