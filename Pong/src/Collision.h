#pragma once
#include "Transform.h"
#include <glm/glm.hpp>

inline glm::vec2 AABBvsAABB(Transform& playerTransform, Transform& ballTransform)
{
    glm::vec2 minA = { playerTransform.Position.x - playerTransform.Size.x / 2, playerTransform.Position.y - playerTransform.Size.y / 2 };
    glm::vec2 maxA = { playerTransform.Position.x + playerTransform.Size.x / 2, playerTransform.Position.y + playerTransform.Size.y / 2 };
    glm::vec2 minB = { ballTransform.Position.x - ballTransform.Size.x / 2, ballTransform.Position.y - ballTransform.Size.y / 2 };
    glm::vec2 maxB = { ballTransform.Position.x + ballTransform.Size.x / 2, ballTransform.Position.y + ballTransform.Size.y / 2 };

    float overlapX = std::min(maxA.x, maxB.x) - std::max(minA.x, minB.x);
    float overlapY = std::min(maxA.y, maxB.y) - std::max(minA.y, minB.y);

    if (overlapX > 0 && overlapY > 0)
    {
        if (overlapX < overlapY)
        {
            if (playerTransform.Position.x < ballTransform.Position.x)
                ballTransform.Position.x += overlapX;
            else
                ballTransform.Position.x -= overlapX;
            FL::SoundPlayer::PlaySound(FL::AssetManager::GetAssets().GetSound("pong"));
            return glm::vec2(true, false);
        }
        else
        {
            if (playerTransform.Position.y < ballTransform.Position.y)
                ballTransform.Position.y += overlapY;
            else
                ballTransform.Position.y -= overlapY;
            return glm::vec2(false, true);
        }
    }
    else
    {
        return glm::vec2(false, false);
    }
}