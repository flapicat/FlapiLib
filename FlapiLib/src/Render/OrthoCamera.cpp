#include "pch.h"
#include "OrthoCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace FL
{
	OrthoCamera::OrthoCamera(float aspectRatio)
		:Camera(aspectRatio)
	{
		m_ProjectionMatrix = glm::ortho(-aspectRatio, aspectRatio,-1.0f,1.0f,m_Near,m_Far);
		m_ViewMatrix = glm::lookAt(m_Pos,m_Front,m_Up);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}