#pragma once
#include "Scene.h"

namespace FL
{
	class Entity
	{
	public:
		Entity(entt::entity e, Scene* scene);
		~Entity();

		template<typename Component, typename ...Args>
		void AddComponent(Args &&... args)
		{
			m_Scene->m_Registry.emplace<Component>(m_e,std::forward<Args>(args)...);
		}

		template<typename Component>
		void RemoveComponent()
		{
			if (this->HasComponent<Component>())
			{
				m_Scene->m_Registry.remove<Component>(m_e);
			}
		}

		template<typename Component>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<Component>(m_e);
		}

		template<typename Component>
		Component& GetComponent()
		{
			return m_Scene->m_Registry.get<Component>(m_e);
		}

		template<typename Component>
		Component* TryGetComponent()
		{
			return m_Scene->m_Registry.try_get<Component>(m_e);
		}

	private:
		entt::entity m_e = { entt::null };
		Scene* m_Scene = nullptr;
	};
};

