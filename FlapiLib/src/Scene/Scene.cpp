#include "pch.h"
#include "Scene.h"

#include <string>
#include <filesystem>

#include "Core/AssetManager.h"
#include "Render/Renderer.h"
#include "Render/Renderer2D.h"

#include "CameraSystem.h"
#include "Components.h"
#include "Entity.h"
#include "ScriptableEntity.h"

namespace FL
{
	Scene* Scene::m_Scene = nullptr;
	std::string Scene::s_SceneFileName = "";

	Scene::Scene()
	{
		m_Scene = this;
	}

	Scene::~Scene()
	{
		if (m_Scene)
			OnDestroy();
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		auto eID = m_Registry.create();
		Entity entity(eID, this);

		std::string eName = name;
		if (eName.empty()) eName = "newEntity";
		static uint32_t SameTagIterator = 0;
		m_Registry.view<TagComponent>().each([this, &eName](auto tag) {
			if (eName == tag.Tag)
			{
				SameTagIterator++;
				eName += "(" + std::to_string(SameTagIterator) + ")";
			}
			});

		entity.AddComponent<TagComponent>(eName);
		entity.AddComponent<TransformComponent>(glm::vec3(0.0f, 0.0f, 0.0f));
		return entity;
	}

	void Scene::DeleteEntity(Entity& entity)
	{
		if(entity)
			m_Registry.destroy(entity.m_entity);
	}

	void Scene::OnInit()
	{
	}

	void Scene::OnDestroy()
	{
		m_Registry.view<NativeScriptingComponent>().each([](auto entity, auto& nsc)
			{
				if (nsc.Instance)
				{
					nsc.Instance->OnDestroy();
					nsc.DestroyScript();
				}
			});
	}

	void Scene::OnUpdate(TimeStep ts)
	{
		CameraSystem::OnUpdate(m_Registry, ts);

		m_Registry.view<NativeScriptingComponent>().each([=](auto entity, auto& nsc)
			{
				if (!nsc.Instance)
				{
					if (nsc.InstantiateScript)
					{
						nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}
				}

				if (nsc.Instance)
					nsc.Instance->OnUpdate(ts);
			});
	}

	void Scene::OnRender()
	{
		CameraComponent* mainCam = nullptr;
		m_Registry.view<CameraComponent>().each([&](auto& cam) {
			if (cam.primary) mainCam = &cam;
			});

		if (!mainCam) return;

		FL::Renderer2D::BeginScene(mainCam->viewProjectionMatrix);
		m_Registry.view<TransformComponent,SpriteComponent2D>().each([](auto& transform,auto& sprite2D)
			{
				if (transform.Rotation != glm::vec3(0.0f))
				{
					FL::Renderer2D::DrawRotatedQuad(transform.Position, transform.Rotation.z, transform.Scale, sprite2D.Color, sprite2D.Texture);
				}
				else
				{
					FL::Renderer2D::DrawQuad(transform.Position, transform.Scale, sprite2D.Color, sprite2D.Texture);
				}
			});
		FL::Renderer2D::EndScene();
	}

	void Scene::OnEvent(Event& e)
	{
		CameraSystem::OnEvent(m_Registry,e);
	}

	void Scene::ForEachEntity(std::function<void(Entity)> callback)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			callback(Entity{ entity, this });
		}
	}

	void Scene::UpdatePrimaryCameraAspect(float aspectRatio)
	{
		auto view = m_Registry.view<CameraComponent, TransformComponent>();
		for (auto entity : view)
		{
			auto& cam = view.get<CameraComponent>(entity);
			auto& transform = view.get<TransformComponent>(entity);

			if (cam.primary)
			{
				cam.aspectRatio = aspectRatio;
				CameraSystem::RecalculateProjectionViewMatrixOfCam(transform, cam);
				break;
			}
		}
	}

	void Scene::LoadScene(std::string& fileName)
	{
		SetUINameFromFIleName(fileName);
		m_Registry.clear();

		s_SceneFileName = fileName;
		YAML::Node root = YAML::LoadFile(fileName);

		if (!root["scene"] || !root["scene"]["entities"])
		{
			LOG_ERROR("Scene file is empty or invalid!");
			return;
		}

		auto entitiesNode = root["scene"]["entities"];
		for (std::size_t i = 0; i < entitiesNode.size(); i++)
		{
			YAML::Node entityNode = entitiesNode[i];
			std::string name = entityNode["name"].as<std::string>();
			Entity entity = CreateEntity(name);

			// Transform
			if (entityNode["transform"])
			{
				auto& transform = entity.GetComponent<TransformComponent>();
				auto position = entityNode["transform"]["position"];
				auto rotation = entityNode["transform"]["rotation"];
				auto scale = entityNode["transform"]["scale"];

				transform.Position = { position[0].as<float>(), position[1].as<float>(), position[2].as<float>() };
				transform.Rotation = { rotation[0].as<float>(), rotation[1].as<float>(), rotation[2].as<float>() };
				transform.Scale = { scale[0].as<float>(), scale[1].as<float>(), scale[2].as<float>() };
			}

			// Sprite
			if (entityNode["sprite2D"])
			{
				auto spriteNode = entityNode["sprite2D"];
				std::string textureName = spriteNode["TextureName"].as<std::string>();

				auto texture = AssetManager::GetAssets().GetTexture(textureName);
				if (!texture)
				{
					LOG_ERROR("Texture not loaded: " + textureName);
				}

				glm::vec4 color(
					spriteNode["Color"][0].as<float>(),
					spriteNode["Color"][1].as<float>(),
					spriteNode["Color"][2].as<float>(),
					spriteNode["Color"][3].as<float>()
				);
				entity.AddComponent<SpriteComponent2D>(color, texture, textureName);
			}

			// Camera
			if (entityNode["camera"])
			{
				auto& transform = entity.GetComponent<TransformComponent>();
				CameraComponent camera;
				auto camNode = entityNode["camera"];
				camera.type = static_cast<CameraTypes>(camNode["type"].as<int>());
				camera.fov = camNode["fov"].as<float>();
				camera.aspectRatio = camNode["aspectRatio"].as<float>();
				camera.nearPlane = camNode["nearPlane"].as<float>();
				camera.farPlane = camNode["farPlane"].as<float>();
				camera.primary = camNode["primary"].as<bool>();

				entity.AddComponent<CameraComponent>(camera);
			}
		}
	}

	void Scene::SetUINameFromFIleName(const std::string& fileName)
	{
		std::string SceneName = std::filesystem::path(fileName).filename().string();
		m_SceneUIname = SceneName;
	}

	void Scene::SaveScene(const std::string& fileName)
	{
		SetUINameFromFIleName(fileName);
		s_SceneFileName = fileName;
		YAML::Node root;
		root["version"] = "1.0";

		auto view = m_Registry.view<TagComponent>();
		for (auto it = view.rbegin(); it != view.rend(); ++it)
		{
			Entity entity{ *it, this };
			root["scene"]["entities"].push_back(SaveEntity(entity));
		}

		std::ofstream file(fileName);
		file << root;
	}

	YAML::Node Scene::SaveEntity(Entity& entity)
	{
		YAML::Node node;
		node["id"] = static_cast<uint32_t>(entity.m_entity);
		node["name"] = entity.GetComponent<TagComponent>().Tag;

		auto transform = entity.GetComponent<TransformComponent>();
		node["transform"]["position"] = std::vector<float>{ transform.Position.x, transform.Position.y, transform.Position.z };
		node["transform"]["rotation"] = std::vector<float>{ transform.Rotation.x, transform.Rotation.y, transform.Rotation.z };
		node["transform"]["scale"]	  = std::vector<float>{ transform.Scale.x,    transform.Scale.y,    transform.Scale.z };

		if (entity.HasComponent<SpriteComponent2D>())
		{
			auto sprite2D = entity.GetComponent<SpriteComponent2D>();
			node["sprite2D"]["TextureName"] = sprite2D.TextureName;
			auto& c = sprite2D.Color;
			node["sprite2D"]["Color"].push_back(c.r);
			node["sprite2D"]["Color"].push_back(c.g);
			node["sprite2D"]["Color"].push_back(c.b);
			node["sprite2D"]["Color"].push_back(c.a);
		}

		if (entity.HasComponent<CameraComponent>())
		{
			auto camera = entity.GetComponent<CameraComponent>();
			node["camera"]["type"] = static_cast<int>(camera.type);

			node["camera"]["fov"] = camera.fov;
			node["camera"]["aspectRatio"] = camera.aspectRatio;
			node["camera"]["nearPlane"] = camera.nearPlane;
			node["camera"]["farPlane"] = camera.farPlane;

			node["camera"]["LastX"] = camera.LastX;
			node["camera"]["LastY"] = camera.LastY;

			node["camera"]["front"] = std::vector<float>{ camera.front.x, camera.front.y, camera.front.z };
			node["camera"]["up"] = std::vector<float>{ camera.up.x, camera.up.y, camera.up.z };
			node["camera"]["right"] = std::vector<float>{ camera.right.x, camera.right.y, camera.right.z };

			node["camera"]["yaw"] = camera.yaw;
			node["camera"]["pitch"] = camera.pitch;
			node["camera"]["moveSpeed"] = camera.moveSpeed;
			node["camera"]["Zoom"] = camera.Zoom;
			node["camera"]["zoomSpeed"] = camera.zoomSpeed;

			node["camera"]["primary"] = camera.primary;
			node["camera"]["ScrollEnabled"] = camera.ScrollEnabled;
			node["camera"]["FirstMouse"] = camera.FirstMouse;
		}

		return node;
	}
}