#pragma once
#include <string>

#include <glm/glm.hpp>

#include "Render/Texture.h"

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
	};

	struct SpriteComponent2D
	{
		Ref<Texture2D> Texture;
		glm::vec4 Color;

		SpriteComponent2D(const glm::vec4& color = glm::vec4(1.0f), Ref<Texture2D> tex = nullptr)
			:Texture(tex), Color(color)
		{

		}
	};
}