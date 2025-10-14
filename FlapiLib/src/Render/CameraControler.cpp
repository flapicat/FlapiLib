#include "pch.h"
#include "CameraControler.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "Core/Input.h"

namespace FL
{

	CameraController::CameraController(float aspectRatio, CameraType type)
		:Camera(aspectRatio, type)
	{

	}
	CameraController::~CameraController()
	{

	}

	void CameraController::OnUpdate(TimeStep ts)
	{
		float velocity = m_CameraSpeed * ts;

		if (m_type == CameraType::Orthographic)
		{
			if (Input::IsKeyPressed(GLFW_KEY_W))
				m_Pos += m_Up * velocity;
			if (Input::IsKeyPressed(GLFW_KEY_S))
				m_Pos -= m_Up * velocity;
			if (Input::IsKeyPressed(GLFW_KEY_A))
				m_Pos -= m_Right * velocity;
			if (Input::IsKeyPressed(GLFW_KEY_D))
				m_Pos += m_Right * velocity;
		}

		if (m_type == CameraType::Perspective)
		{
			if (Input::IsKeyPressed(GLFW_KEY_W))
				m_Pos += m_Front * velocity;
			if (Input::IsKeyPressed(GLFW_KEY_S))
				m_Pos -= m_Front * velocity;
			if (Input::IsKeyPressed(GLFW_KEY_E))
				m_Pos += m_Up * velocity;
			if (Input::IsKeyPressed(GLFW_KEY_Q))
				m_Pos -= m_Up * velocity;
			if (Input::IsKeyPressed(GLFW_KEY_A))
				m_Pos -= m_Right * velocity;
			if (Input::IsKeyPressed(GLFW_KEY_D))
				m_Pos += m_Right * velocity;
		}

		m_ViewMatrix = glm::lookAt(m_Pos, m_Pos + m_Front, m_Up);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void CameraController::OnMouseScrolled(const MouseScrollEvent& e)
	{
		if (m_type == CameraType::Orthographic)
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

		if (m_type == CameraType::Perspective)
		{
			m_Fov -= e.GetYoffset() * m_ZoomSpeed;
			m_Fov = glm::clamp(m_Fov, 15.0f, 90.0f);

			m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_Near, m_Far);
			m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
	}

	void CameraController::OnMouseMoved(const MouseMovedEvent& e)
	{
		if (m_type == CameraType::Perspective)
		{
			float xpos = e.GetxPos();
			float ypos = e.GetyPos();

			if (m_FirstMouse)
			{
				m_LastX = xpos;
				m_LastY = ypos;
				m_FirstMouse = false;
			}

			float xOffset = xpos - m_LastX;
			float yOffset = m_LastY - ypos; 
			m_LastX = xpos;
			m_LastY = ypos;

			const float sensitivity = 0.1f;
			xOffset *= sensitivity;
			yOffset *= sensitivity;

			m_Yaw += xOffset;
			m_Pitch += yOffset;

			m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);

			glm::vec3 front;
			front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			front.y = sin(glm::radians(m_Pitch));
			front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			m_Front = glm::normalize(front);

			m_Right = glm::normalize(glm::cross(m_Front, m_Up));
		}
	}
}