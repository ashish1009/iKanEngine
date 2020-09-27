#pragma once
#include <iKan/Renderer/VertexArray.h>
#include <iKan/Renderer/Shader.h>
#include <iKan/Renderer/Texture.h>

namespace iKan {
    
    class Mesh
    {
    public:
        struct MeshVertex {
            glm::vec3 Position;
            glm::vec3 Normal;
            glm::vec2 TexCoords;
            glm::vec3 Tangent;
            glm::vec3 Bitangent;
        };
        
        struct MeshTexture {
            Ref<Texture> Texture;
            std::string  Type;
            std::string  Path;
        };
        
    public:
        Mesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices, std::vector<MeshTexture> textures);
        
        void Draw(Shader &shader);
        
    private:
        std::vector<MeshVertex>     m_Vertices;
        std::vector<uint32_t>       m_Indices;
        std::vector<MeshTexture>    m_Textures;
        //        uint32_t                    VAO;
        Ref<VertexArray>            m_VAO;
        
    private:
        void setupMesh();
    };
    
}
