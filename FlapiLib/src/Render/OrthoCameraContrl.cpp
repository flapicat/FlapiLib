#include "pch.h"
#include "OrthoCameraContrl.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "Core/Input.h"

namespace FL
{
	OrthoCameraContrl::OrthoCameraContrl(float aspectRatio)
		: OrthoCamera(aspectRatio)
	{
	}

	OrthoCameraContrl::~OrthoCameraContrl()
	{
	}

	void OrthoCameraContrl::OnUpdate(TimeStep ts)
	{
		GLFWwindow* window = glfwGetCurrentContext();

		float velocity = m_CameraSpeed * ts;
		
		if (Input::IsKeyPressed(GLFW_KEY_W))
			m_Pos += m_Up * velocity;
		if (Input::IsKeyPressed(GLFW_KEY_S))
			m_Pos -= m_Up * velocity;
		if (Input::IsKeyPressed(GLFW_KEY_A))
			m_Pos -= m_Right * velocity;
		if (Input::IsKeyPressed(GLFW_KEY_D))
			m_Pos += m_Right * velocity;

		m_ViewMatrix = glm::lookAt(m_Pos, m_Pos + m_Front, m_Up);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthoCameraContrl::OnMouseScrolled(const MouseScrollEvent& e)
	{
		m_Zoom -= e.GetYoffset() * m_ZoomSpeed; 
		m_Zoom = glm::clamp(m_Zoom, 0.25f, 4.0f); 

		float orthoLeft = -m_AspectRatio * m_Zoom;
		float orthoRight = m_AspectRatio * m_Zoom;
		float orthoBottom = -1.0f * m_Zoom;
		float orthoTop = 1.0f * m_Zoom;

		m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_Near, m_Far);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}