#pragma once
#include "Core/AssetManager.h"
#include "Core/Input.h"
#include "Scene/Components.h"
#include "Scene/ScriptableEntity.h"

namespace FL
{
	static std::vector<std::string> scriptNames = { "None", "CameraControllerScript" };

	class DummyScript : public  ScriptableEntity
	{
	public:
		DummyScript() = default;
		void OnCreate() override
		{
		}

		void OnUpdate(TimeStep ts) override
		{
		}
	};

	class CameraControllerScript : public  ScriptableEntity
	{
	public:
		CameraControllerScript() = default;
		void OnCreate() override
		{

		}

		void OnUpdate(TimeStep ts) override
		{
			if (!HasComponent<CameraComponent>()) return;
			auto& cam = GetComponent<CameraComponent>();
			auto& transform = GetComponent<TransformComponent>();
			float velocity = cam.moveSpeed * ts;
			if (cam.type == CameraTypes::Orthographic)
			{
				if (Input::IsKeyPressed(GLFW_KEY_W))
					transform.Position += cam.up * velocity;
				if (Input::IsKeyPressed(GLFW_KEY_S))
					transform.Position -= cam.up * velocity;
				if (Input::IsKeyPressed(GLFW_KEY_A))
					transform.Position -= cam.right * velocity;
				if (Input::IsKeyPressed(GLFW_KEY_D))
					transform.Position += cam.right * velocity;
			}

			if (cam.type == CameraTypes::Perspective)
			{
				if (Input::IsKeyPressed(GLFW_KEY_W))
					transform.Position += cam.front * velocity;
				if (Input::IsKeyPressed(GLFW_KEY_S))
					transform.Position -= cam.front * velocity;
				if (Input::IsKeyPressed(GLFW_KEY_E))
					transform.Position += cam.up * velocity;
				if (Input::IsKeyPressed(GLFW_KEY_Q))
					transform.Position -= cam.up * velocity;
				if (Input::IsKeyPressed(GLFW_KEY_A))
					transform.Position -= cam.right * velocity;
				if (Input::IsKeyPressed(GLFW_KEY_D))
					transform.Position += cam.right * velocity;
			}
		}
	};
}