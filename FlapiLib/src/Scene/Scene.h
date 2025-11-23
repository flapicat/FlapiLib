#pragma once
#include "Core/Core.h"
#include "Event/Events.h"

#include <entt/entt.hpp>
#include <yaml-cpp/yaml.h>

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

		void SaveScene(const std::string& fileName);
		void LoadScene(std::string& fileName);
	
		std::string& GetName() { return m_SceneUIname; }
		void SetUINameFromFIleName(const std::string& fileName);
	public:
		static std::string s_SceneFileName;
	protected:
		static Scene* m_Scene;
		entt::registry m_Registry;
		std::string m_SceneUIname = "";

		friend class Entity;
	private:
		YAML::Node SaveEntity(Entity& entity);
	};

}