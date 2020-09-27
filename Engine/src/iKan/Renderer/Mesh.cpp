#include <iKan/Renderer/Mesh.h>
#include <iKan/Renderer/Renderer.h>

namespace iKan {
    
    Mesh::Mesh(std::vector<Mesh::MeshVertex> vertices, std::vector<uint32_t> indices, std::vector<Mesh::MeshTexture> textures)
    : m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
    {
        setupMesh();
    }
    
    void Mesh::Draw(Shader &shader)
    {
        uint32_t diffuseNr  = 1;
        uint32_t specularNr = 1;
        uint32_t normalNr   = 1;
        uint32_t heightNr   = 1;
        
        uint32_t slot = 0;
        for(auto texture : m_Textures)
        {
            std::string number;
            std::string name = texture.Type;
            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++);
            else if(name == "texture_normal")
                number = std::to_string(normalNr++);
            else if(name == "texture_height")
                number = std::to_string(heightNr++);
            
            /* now set the sampler to the correct texture unit */
            shader.Bind();
            shader.SetUniformInt1((name + number).c_str(), slot);
            
            /* and finally bind the texture */
            texture.Texture->Bind(slot);
            slot++;
        }
        
        m_VAO->Bind();
        Renderer::DrawIndexed(m_VAO);
        m_VAO->Unbind();
    }
    
    void Mesh::setupMesh()
    {
        m_VAO = VertexArray::Create();
        
        Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(uint32_t(m_Vertices.size() * sizeof(Mesh::MeshVertex)), (float*)(&m_Vertices[0]));
        vertexBuffer->AddLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float3, "a_Tangent" },
            { ShaderDataType::Float3, "a_Bitangent" }
        });
        m_VAO->AddVertexBuffer(vertexBuffer);
        
        Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(uint32_t(m_Indices.size()), (uint32_t*)(&m_Indices[0]));
        m_VAO->SetIndexBuffer(indexBuffer);
        
        m_VAO->Unbind();
    }
}
