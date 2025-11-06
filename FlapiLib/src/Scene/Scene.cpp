#include "pch.h"
#include "Scene.h"

#include "Core/AssetManager.h"
#include "Render/Renderer2D.h"

#include "Entity.h"
#include "Components.h"

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

	void Scene::OnInit(Camera& usedCam)
	{
		s_data.CurUsedCamera = &usedCam;


		// --- Entity 1: Red box ---
		{
			FL::Entity e = this->CreateEntity("RedBox");
			e.AddComponent<FL::TransformComponent>(
				glm::vec3(-1.0f, 0.0f, 0.0f),   // position
				glm::vec3(0.0f, 0.0f, 25.0f),   // rotation (z=25 degrees)
				glm::vec3(1.0f, 1.0f, 1.0f));   // scale (normal size)
			e.AddComponent<FL::SpriteComponent2D>(
				glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
				FL::AssetManager::GetAssets().GetTexture("container"));
		}

		// --- Entity 2: Green box ---
		{
			FL::Entity e = this->CreateEntity("GreenBox");
			e.AddComponent<FL::TransformComponent>(
				glm::vec3(1.0f, 0.5f, 0.0f),    // position
				glm::vec3(0.0f, 0.0f, 45.0f),   // rotation (z=45 degrees)
				glm::vec3(0.75f, 0.75f, 1.0f)); // smaller
			e.AddComponent<FL::SpriteComponent2D>(
				glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
				FL::AssetManager::GetAssets().GetTexture("container"));
		}

		// --- Entity 3: Blue box ---
		{
			FL::Entity e = this->CreateEntity("BlueBox");
			e.AddComponent<FL::TransformComponent>(
				glm::vec3(0.0f, 1.0f, 0.0f),    // position
				glm::vec3(0.0f, 0.0f, -15.0f),  // rotation (negative)
				glm::vec3(1.5f, 1.5f, 1.0f));   // larger
			e.AddComponent<FL::SpriteComponent2D>(
				glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
				FL::AssetManager::GetAssets().GetTexture("container"));
		}

		// --- Entity 4: White box ---
		{
			FL::Entity e = this->CreateEntity("WhiteBox");
			e.AddComponent<FL::TransformComponent>(
				glm::vec3(0.0f, -1.0f, 0.0f),   // position
				glm::vec3(0.0f, 0.0f, 5.0f),    // slight rotation
				glm::vec3(0.5f, 0.5f, 1.0f));   // small scale
			e.AddComponent<FL::SpriteComponent2D>(
				glm::vec4(1.0f),
				FL::AssetManager::GetAssets().GetTexture("container"));
		}
	}

	void Scene::OnDestroy()
	{

	}

	void Scene::OnUpdate(TimeStep ts)
	{
		m_Registry.view<TransformComponent>().each([ts](auto& transform) {

			});

	}

	void Scene::OnRender()
	{
		if (!s_data.CurUsedCamera)
			return;

		FL::Renderer2D::BeginScene(*s_data.CurUsedCamera);
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

	}
}