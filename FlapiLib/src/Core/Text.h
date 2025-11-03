#pragma once
#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "Render/Shader.h"
#include "Render/VertexArray.h"
#include "Render/Buffer.h"

#include "Font.h"

namespace FL
{
    class Text
    {
    public:
        Text(std::shared_ptr<Font> font, 
            const std::string& content,
            const glm::vec2& position, 
            float scale = 1.0f, 
            const glm::vec3& color = { 1.0f, 1.0f, 1.0f });

        void SetText(const std::string& text) { m_Text = text; }
        void SetColor(const glm::vec3& color) { m_Color = color; }
        void SetPosition(const glm::vec2& pos) { m_Position = pos; }
        void SetScale(float scale) { m_Scale = scale; }
        void SetFont(std::shared_ptr<Font> font) { m_Font = font; }

        void Render(Ref<Shader> shader);
    private:
        std::shared_ptr<Font> m_Font;
        std::string m_Text;
        glm::vec2 m_Position;
        glm::vec3 m_Color;
        float m_Scale;

        Ref<VertexArray> VA = nullptr;
        Ref<VertexBuffer> VB = nullptr;
    };

}