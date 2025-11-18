#pragma once
#include "Core/Core.h"
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
		void DeleteEntity(Entity& entity);

		void OnInit();
		void OnDestroy();
		void OnUpdate(TimeStep ts);
		void OnRender();
		void OnEvent(Event& e);

		void ForEachEntity(std::function<void(Entity)> callback);
		
		void UpdatePrimaryCameraAspect(float aspectRatio);
	protected:
		static Scene* m_Scene;
		entt::registry m_Registry;

		friend class Entity;
	};

}