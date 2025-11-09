#pragma once
#include "Event/Events.h"

#include <entt/entt.hpp>

#include "Core/TimeStep.h"

namespace FL
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "");

		void OnInit();
		void OnDestroy();
		void OnUpdate(TimeStep ts);
		void OnRender();
		void OnEvent(Event& e);
	protected:
		static Scene* m_Scene;
		entt::registry m_Registry;

		friend class Entity;
	};

}