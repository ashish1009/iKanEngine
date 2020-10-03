#pragma once

#include <iKan/Renderer/VertexArray.h>
#include <iKan/Renderer/Shader.h>
#include <iKan/Renderer/Texture.h>

// Assimp Library
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

namespace iKan {
    
    struct MeshVertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };
    
    struct MeshTexture
    {
        Ref<Texture> Texture;
        std::string  Type;
        std::string  Path;
    };
    
    class SubMesh
    {
    public:
        
    public:
        SubMesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices, std::vector<MeshTexture> textures);
        
        void Draw(Shader &shader);
        
    private:
        std::vector<MeshVertex>     m_Vertices;
        std::vector<uint32_t>       m_Indices;
        std::vector<MeshTexture>    m_Textures;
        Ref<VertexArray>            m_VAO;
        
    private:
        void setupMesh();
    };
    
    class Mesh : public RefCounted
    {
    public:
        Mesh(const std::string& path);
        ~Mesh() = default;
        
        void Draw(Shader &shader) const;
        
    private:
        void LoadModel(const std::string& path);
        void ProcessNode(aiNode* node, const aiScene* scene);
        SubMesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<MeshTexture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
        
    private:
        std::vector<SubMesh>           m_Meshes;
        std::string                 m_Directory;
        std::vector<MeshTexture>  m_TexturesLoaded;
    };
    
}
