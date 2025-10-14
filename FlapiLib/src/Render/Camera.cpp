#include "pch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace FL
{
	Camera::Camera(float aspectRatio, CameraType type)
		:m_AspectRatio(aspectRatio), m_type(type)
	{
		SetUpCamera();
	}
	
	void Camera::SetUpCamera()
	{
		if (m_type == CameraType::Orthographic)
		{
			m_Near = -1;
			m_Far = 1;
			m_ProjectionMatrix = glm::ortho(-m_AspectRatio, m_AspectRatio, -1.0f, 1.0f, m_Near, m_Far);
		}

		if (m_type == CameraType::Perspective)
		{
			m_Near = 0.1;
			m_Far = 1000;
			m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_Near, m_Far);
		}

		m_ViewMatrix = glm::lookAt(m_Pos, m_Pos + m_Front, m_Up);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}