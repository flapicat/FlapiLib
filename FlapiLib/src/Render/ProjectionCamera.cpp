#include "pch.h"
#include "ProjectionCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace FL
{
	ProjectionCamera::ProjectionCamera(float aspectRatio)
		:Camera(aspectRatio)
	{
		m_type = CameraType::Perspective;
		m_Near = 0.1;
		m_Far = 1000;
		m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), aspectRatio, m_Near, m_Far);
		m_ViewMatrix = glm::lookAt(m_Pos, m_Pos + m_Front, m_Up);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}