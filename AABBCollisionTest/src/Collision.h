#pragma once
#include "Transform.h"


inline bool AABBvsAABB(Transform& t1, Transform& t2)
{
	glm::vec2 minA = { t1.Position.x - t1.Size.x / 2, t1.Position.y - t1.Size.y / 2 };
	glm::vec2 maxA = { t1.Position.x + t1.Size.x / 2, t1.Position.y + t1.Size.y / 2 };
	glm::vec2 minB = { t2.Position.x - t2.Size.x / 2, t2.Position.y - t2.Size.y / 2 };
	glm::vec2 maxB = { t2.Position.x + t2.Size.x / 2, t2.Position.y + t2.Size.y / 2 };

	float overlapX = std::min(maxA.x, maxB.x) - std::max(minA.x, minB.x);
	float overlapY = std::min(maxA.y, maxB.y) - std::max(minA.y, minB.y);
	
	if (overlapX > 0 && overlapY > 0)
	{
        if (overlapX < overlapY)
        {
            if (t1.Position.x < t2.Position.x)
                t1.Position.x -= overlapX;
            else
                t1.Position.x += overlapX;
        }
        else
        {
            if (t1.Position.y < t2.Position.y)
                t1.Position.y -= overlapY;
            else
                t1.Position.y += overlapY; 
        }
	}

	if (maxA.x < minB.x || maxB.x < minA.x) return false;
	if (maxA.y < minB.y || maxB.y < minA.y) return false;
	return true;
}