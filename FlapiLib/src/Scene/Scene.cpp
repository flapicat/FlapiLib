#include "pch.h"
#include "Scene.h"

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
		std::string eName = name;
		if (eName.empty()) eName = "newEntity";
		auto eID = m_Registry.create();
		Entity entity(eID, this);
		entity.AddComponent<TagComponent>(eName);
		return entity;
	}

	void Scene::OnInit()
	{
		
		// --- Entity 2: Green box ---
		{
			FL::Entity e = this->CreateEntity("GreenBox");
			e.AddComponent<FL::TransformComponent>(
				glm::vec3(1.0f, 0.5f, 0.0f),   
				glm::vec3(0.0f, 0.0f, 45.0f),  
				glm::vec3(0.75f, 0.75f, 1.0f));
			e.AddComponent<FL::SpriteComponent2D>(
				glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
				FL::AssetManager::GetAssets().GetTexture("container"));
		}

		// --- Entity 3: Blue box ---
		{
			FL::Entity e = this->CreateEntity("BlueBox");
			e.AddComponent<FL::TransformComponent>(
				glm::vec3(0.0f, 1.0f, 0.0f),  
				glm::vec3(0.0f, 0.0f, -15.0f),
				glm::vec3(1.5f, 1.5f, 1.0f)); 
			e.AddComponent<FL::SpriteComponent2D>(
				glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
				FL::AssetManager::GetAssets().GetTexture("container"));
		}

		// --- Entity 4: White box ---
		{
			FL::Entity e = this->CreateEntity("WhiteBox");
			e.AddComponent<FL::TransformComponent>(
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 5.0f), 
				glm::vec3(0.5f, 0.5f, 1.0f));
			e.AddComponent<FL::SpriteComponent2D>(
				glm::vec4(1.0f),
				FL::AssetManager::GetAssets().GetTexture("container"));
		}
		//// --- Entity 5: Model ---
		//{
		//	FL::Entity e = this->CreateEntity("Model");
		//	e.AddComponent<FL::TransformComponent>();
		//	e.AddComponent<FL::Model3DComponent>(
		//		FL::AssetManager::GetAssets().GetModel("backpack"));
		//}
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
					nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}

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

		//FL::Renderer::BeginScene(mainCam->viewProjectionMatrix);
		//m_Registry.view<TransformComponent, Model3DComponent>().each([](auto& transform, auto& Model3D)
		//	{
		//		FL::Renderer::SubmitModel(Model3D.model_, transform.GetTransform());
		//	});
		//FL::Renderer::EndScene();
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
}