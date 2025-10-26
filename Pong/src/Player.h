#pragma once
#include "FlapiLib.h"
#include <glm/glm.hpp>

#include <Core/TimeStep.h>

class Player
{
public:
	enum PlayerNum
	{
		Player1,
		Player2
	};
	struct Transform
	{
		glm::vec3 Position = {0.0f,0.0f,0.0f};
		glm::vec3 Size = {0.05f,0.5f,0.0f};
	};
public:
	Player(PlayerNum playerNum);
	~Player();

	void OnUpdate(FL::TimeStep ts);
	
	Transform& GetTransform() { return m_Transform; }
	void SetTransform(const Transform& transform) { m_Transform = transform; }

	void OnWindowResize(FL::Camera camera);
private:
	PlayerNum m_PLayerNum = Player1;
	Transform m_Transform;
	float m_playerSpeed = 2.0f;
};

