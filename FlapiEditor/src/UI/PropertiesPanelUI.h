#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Scene/Entity.h"

namespace FL
{
	void DrawPropertiesPanel();
	void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float dragSpeed = 0.01f);
	void DrawTagComponent();
	void DrawTransformComponent();
	void DrawSprite2DComponent();
	void DrawCameraComponent();
}