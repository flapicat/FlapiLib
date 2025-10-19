#include "pch.h"
#include "Mesh.h"

namespace FL
{

    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
        :m_vertices(vertices), m_indices(indices), m_textures(textures)
    {
        setupMesh();
    }


    void Mesh::Draw(Ref<Shader> shader) //IDK how to refactor this func
    {
        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < m_textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = m_textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to string
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to string
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct texture unit
            shader->setInt((name + number).c_str(), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
        }

        m_VA->Bind();
        glDrawElements(GL_TRIANGLES, m_VA->GetIB()->GetCount(), GL_UNSIGNED_INT, 0);
        m_VA->Unbind();

        glActiveTexture(GL_TEXTURE0);
    }

    void Mesh::setupMesh()
    {
        //VA
        m_VA = VertexArray::Create();
        m_VA->Bind();

        //VB
        Ref<VertexBuffer> VB = VertexBuffer::Create(&m_vertices[0], m_vertices.size() * sizeof(Vertex));
        BufferLayout  layout =
        {
            { ShaderType::Float3, "aPos"},
            { ShaderType::Float3, "aNormal"},
            { ShaderType::Float2, "aTexCoords"},
            { ShaderType::Float3, "tangent"},
            { ShaderType::Float3, "bitangent"},
            { ShaderType::Float4, "ids"},
            { ShaderType::Float4, "weights"},
        };
        VB->SetLayout(layout);
        m_VA->SetVB(VB);

        //IB
        Ref<IndexBuffer> IB = IndexBuffer::Create(&m_indices[0], m_indices.size());
        m_VA->SetIB(IB);
        m_VA->Unbind();
    }

}