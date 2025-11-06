#pragma once
#include "Event/Events.h"

#include <entt/entt.hpp>

#include "Core/TimeStep.h"
#include "Render/Camera.h"

namespace FL
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "");

		void OnInit(Camera& usedCam);
		void OnDestroy();
		void OnUpdate(TimeStep ts);
		void OnRender();
		void OnEvent(Event& e);
	protected:
		static Scene* m_Scene;
		entt::registry m_Registry;

		struct SceneData
		{
			Camera* CurUsedCamera = nullptr;
		};
		SceneData s_data;

		friend class Entity;
	};

}