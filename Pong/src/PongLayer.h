#pragma once
#include "FlapiLib.h"

#include <glad/glad.h>
#include <ImGui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Player.h"

class PongLayer : public FL::Layer
{
public:
	PongLayer();
	~PongLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(FL::TimeStep ts) override;
	virtual void OnRender() override;
	virtual void OnEvent(FL::Event& e) override;
	virtual void OnImGuiRender() override;

private:
	float m_fps = 0.0f;
	FL::CameraController m_Camera;
	Player m_Player1;
	Player m_Player2;
};

