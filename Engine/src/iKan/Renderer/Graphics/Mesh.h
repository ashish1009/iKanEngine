#pragma once

#include <iKan/Renderer/Graphics/Bone.h>
#include <iKan/Renderer/Graphics/VertexArray.h>
#include <iKan/Renderer/Graphics/Shader.h>
#include <iKan/Renderer/Graphics/Texture.h>

#include <iKan/Scene/Entity.h>

// Assimp Library
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

namespace iKan {

    struct VertexBone
    {
        static const uint32_t WEIGHTS_PER_VERTEX = 4;

        uint32_t ID;
        float    Weight;
    };
    
    struct MeshVertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;

        VertexBone BoneData[VertexBone::WEIGHTS_PER_VERTEX];

        int32_t ObjectID;
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
        Mesh(const std::string& path, Entity entity);
        ~Mesh() = default;
        
        const std::string& GetFilepath() const { return m_Filepath; }
        
        void Draw(Shader &shader) const;
                
    private:
        void LoadModel(const std::string& path);
        void ProcessNode(aiNode* node);
        SubMesh ProcessMesh(aiMesh *mesh);
        std::vector<MeshTexture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

        void RecursiveNodeProcess(aiNode* node);
        void AnimNodeProcess();

        Bone* FindBone(const std::string& name);
        aiNode* FindAiNode(const std::string& name);
        aiNodeAnim* FindAiNodeAnim(const std::string& name);
        int32_t FindBoneIDByName(const std::string& name);

    private:
        std::string               m_Filepath;
        std::string               m_Directory;
        std::vector<SubMesh>      m_Meshes;
        std::vector<MeshTexture>  m_TexturesLoaded;

        std::vector<aiNode*>      m_AiNodes;
        std::vector<aiNodeAnim*>  m_AiNodesAnim;
        const aiScene*            m_Scene;
        glm::mat4                 m_GlobalInverseTransform;

        std::vector<Bone>         m_Bones;

        Entity                    m_Entity;
    };
    
}
