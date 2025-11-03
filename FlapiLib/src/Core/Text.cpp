#include "pch.h"
#include "Text.h"

#include <glad/glad.h>

namespace FL
{
    Text::Text(std::shared_ptr<Font> font, const std::string& content,
        const glm::vec2& position, float scale, const glm::vec3& color)
        : m_Font(font), m_Text(content), m_Position(position), m_Scale(scale), m_Color(color)
    {
        if (VA == nullptr)
        {
            VA = VertexArray::Create();
            VB = VertexBuffer::Create(nullptr, sizeof(float) * 6 * 4);
            BufferLayout layout = {
                {ShaderType::Float4, "vertex"}
            };
            VB->SetLayout(layout);
            VA->SetVB(VB);
        }
    }

    void Text::Render(Ref<Shader> shader)
    {
        shader->Use();
        shader->setVec3("textColor", m_Color);
        glActiveTexture(GL_TEXTURE0);
        VA->Bind();

        float x = m_Position.x;
        float y = m_Position.y;

        for (auto c = m_Text.begin(); c != m_Text.end(); c++) {
            auto& chars = m_Font->GetCharacters();
            auto it = chars.find(*c);
            if (it == chars.end())
                continue;
            Character ch = it->second;

            float xpos = x + ch.Bearing.x * m_Scale;
            float ypos = y + (m_Font->GetSize() - ch.Bearing.y) * m_Scale;

            float w = ch.Size.x * m_Scale;
            float h = ch.Size.y * m_Scale;

            float vertices[6][4] = {
                {xpos,     ypos,       0.0f, 1.0f},
                {xpos,     ypos + h,   0.0f, 0.0f},
                {xpos + w, ypos + h,   1.0f, 0.0f},

                {xpos,     ypos,       0.0f, 1.0f},
                {xpos + w, ypos + h,   1.0f, 0.0f},
                {xpos + w, ypos,       1.0f, 1.0f}
            };

            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            VB->SetBufferData(vertices, sizeof(vertices));
            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += (ch.Advance >> 6) * m_Scale;
        }

        VA->Unbind();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}