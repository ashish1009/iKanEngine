#pragma once

#include <iKan/Core/TimeStep.h>

#include <iKan/Renderer/VertexArray.h>
#include <iKan/Renderer/Buffers.h>
#include <iKan/Renderer/Shader.h>
#include <iKan/Renderer/Texture.h>
#include <iKan/Renderer/Material.h>

struct aiNode;
struct aiAnimation;
struct aiNodeAnim;
struct aiScene;

namespace Assimp {
    class Importer;
}

namespace iKan {

    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec3 Tangent;
        glm::vec3 Binormal;
        glm::vec2 Texcoord;
    };
    
    struct AnimatedVertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec3 Tangent;
        glm::vec3 Binormal;
        glm::vec2 Texcoord;
        
        uint32_t IDs[4] = { 0, 0,0, 0 };
        float Weights[4]{ 0.0f, 0.0f, 0.0f, 0.0f };
        
        void AddBoneData(uint32_t BoneID, float Weight)
        {
            for (size_t i = 0; i < 4; i++)
            {
                if (Weights[i] == 0.0)
                {
                    IDs[i] = BoneID;
                    Weights[i] = Weight;
                    return;
                }
            }
            
            // TODO: Keep top weights
            IK_CORE_WARN("Vertex has more than four bones/weights affecting it, extra data will be discarded (BoneID={0}, Weight={1})", BoneID, Weight);
        }
    };
    
    static const int NumAttributes = 5;
    
    struct Index
    {
        uint32_t V1, V2, V3;
    };
    
    static_assert(sizeof(Index) == 3 * sizeof(uint32_t));
    
    struct BoneInfo
    {
        glm::mat4 BoneOffset;
        glm::mat4 FinalTransformation;
    };
    
    struct VertexBoneData
    {
        uint32_t IDs[4];
        float Weights[4];
        
        VertexBoneData()
        {
            memset(IDs, 0, sizeof(IDs));
            memset(Weights, 0, sizeof(Weights));
        };
        
        void AddBoneData(uint32_t BoneID, float Weight)
        {
            for (size_t i = 0; i < 4; i++)
            {
                if (Weights[i] == 0.0)
                {
                    IDs[i] = BoneID;
                    Weights[i] = Weight;
                    return;
                }
            }
            
            // should never get here - more bones than we have space for
            IK_CORE_ASSERT(false, "Too many bones!");
        }
    };
    
    struct Triangle
    {
        Vertex V0, V1, V2;
        
        Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
        : V0(v0), V1(v1), V2(v2) {}
    };
    
    struct AABB
    {
        glm::vec3 Min, Max;
        
        AABB()
        : Min(0.0f), Max(0.0f) {}
        
        AABB(const glm::vec3& min, const glm::vec3& max)
        : Min(min), Max(max) {}
        
    };

    class Submesh
    {
    public:
        uint32_t BaseVertex;
        uint32_t BaseIndex;
        uint32_t MaterialIndex;
        uint32_t IndexCount;
        
        glm::mat4 Transform;
        AABB BoundingBox;
        
        std::string NodeName, MeshName;
    };
    
    class Mesh
    {
    public:
        Mesh(const std::string& filename);
        ~Mesh();

    private:
        std::vector<Submesh>    m_Submeshes;
        Scope<Assimp::Importer> m_Importer;
        
        glm::mat4 m_InverseTransform;
        
        uint32_t m_BoneCount = 0;
        std::vector<BoneInfo> m_BoneInfo;
        
        Ref<VertexArray>  m_Pipeline;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer>  m_IndexBuffer;
        
        std::vector<Vertex>                         m_StaticVertices;
        std::vector<AnimatedVertex>                 m_AnimatedVertices;
        std::vector<Index>                          m_Indices;
        std::unordered_map<std::string, uint32_t>   m_BoneMapping;
        std::vector<glm::mat4>                      m_BoneTransforms;
        
        const aiScene* m_Scene;
        
        // Materials
        Ref<Shader>                         m_MeshShader;
        Ref<Material>                       m_BaseMaterial;
        std::vector<Ref<Texture>>         m_Textures;
        std::vector<Ref<Texture>>         m_NormalMaps;
        std::vector<Ref<MaterialInstance>>  m_Materials;
        
        std::unordered_map<uint32_t, std::vector<Triangle>> m_TriangleCache;
        
        // Animation
        bool  m_IsAnimated       = false;
        bool  m_AnimationPlaying = true;
        float m_AnimationTime    = 0.0f;
        float m_WorldTime        = 0.0f;
        float m_TimeMultiplier   = 1.0f;
        
        std::string m_FilePath;
    };
    
}
