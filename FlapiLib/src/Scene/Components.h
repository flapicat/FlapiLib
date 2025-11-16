#pragma once
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Render/Texture.h"
#include "Core/App.h"
#include "Model/Model.h"

namespace FL
{
	struct TagComponent {
		std::string Tag;
	};

	struct TransformComponent
	{
		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;

		TransformComponent(const glm::vec3 position = { 0.0f,0.0f,0.0f }, const glm::vec3 rotation = { 0.0f,0.0f,0.0f }, const glm::vec3 scale = {1.0f,1.0f,1.0f})
			:Position(position), Rotation(rotation), Scale(scale)
		{
		}

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), Position) *
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), glm::vec3(1, 0, 0)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0, 1, 0)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(0, 0, 1)) *
				glm::scale(glm::mat4(1.0f), Scale);;
		}
	};

	struct SpriteComponent2D
	{
		Ref<Texture2D> Texture;
		std::string TextureName;
		glm::vec4 Color;

		SpriteComponent2D(const glm::vec4& color = glm::vec4(1.0f), Ref<Texture2D> tex = nullptr, const std::string& textureName = "None")
			:Texture(tex), Color(color), TextureName(textureName)
		{
		}
	};

	enum class CameraTypes
	{
		Orthographic, Perspective
	};

	struct CameraComponent {

		CameraTypes type = CameraTypes::Perspective;

		float fov = 60.0f;
		float aspectRatio = 16.0f / 9.0f;
		float nearPlane = -1.0f;
		float farPlane = 1.0f;
		
		float LastX = 0;
		float LastY = 0;

		glm::mat4 viewMatrix{ 1.0f };
		glm::mat4 projectionMatrix{ 1.0f };
		glm::mat4 viewProjectionMatrix{ 1.0f };

		glm::vec3 front = { 0.0f, 0.0f, -1.0f };
		glm::vec3 up = { 0.0f, 1.0f, 0.0f };
		glm::vec3 right = { 1.0f, 0.0f, 0.0f };

		float yaw = -90.0f;
		float pitch = 0.0f;

		float moveSpeed = 5.0f;
		float Zoom = 1.0f;
		float zoomSpeed = 1.0f;

		bool primary = false; 
		bool ScrollEnabled = false;
		bool FirstMouse = true;

		CameraComponent(CameraTypes CamType = CameraTypes::Orthographic)
		{
			static auto& window = App::Get().GetWindow();
			aspectRatio = (float)window.GetWidth() / (float)window.GetHeight();

			if (CamType == CameraTypes::Orthographic)
			{
				nearPlane = -1.0f;
				farPlane = 1.0f;
				projectionMatrix = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, nearPlane, farPlane);
			}
			if (CamType == CameraTypes::Perspective)
			{
				nearPlane = 0.1f;
				farPlane = 1000.0f;
				projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
			}
		}

		void CameraComponent::OnTypeChange()
		{
			if (type == CameraTypes::Orthographic)
			{
				nearPlane = -1.0f;
				farPlane = 1.0f;
				projectionMatrix = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, nearPlane, farPlane);
			}
			if (type == CameraTypes::Perspective)
			{
				nearPlane = 0.1f;
				farPlane = 1000.0f;
				projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
			}
			yaw = -90.0f;
			pitch = 0.0f;
		}
	};

	struct NativeScriptingComponent
	{
		std::function<void()> InstantiateScript;
		std::function<void()> DestroyScript;

		class ScriptableEntity* Instance = nullptr;

		template<typename T>
		void Bind()
		{
			InstantiateScript = [this]() {Instance = new T(); };
			DestroyScript = [this]() {delete (T*)Instance; Instance = nullptr; };
		}
	};

	struct Model3DComponent
	{
		Ref<Model> model_;

		Model3DComponent(Ref<Model> model = nullptr)
			:model_(model)
		{
		}
	};
}