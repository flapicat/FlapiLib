#pragma once

#include <FlapiLib.h>

inline static void DrawBoard()
{
	float yPos = 1.0f;
	float ySize = 0.2;
	while (yPos >= -1)
	{
		FL::Renderer2D::DrawQuad({ 0.0,yPos - ySize/2, -0.1 }, { 0.03,ySize }, glm::vec4(0.5, 0.5, 0.5, 1.0));
		yPos -= 0.3;
	}
		
}