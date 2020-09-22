#include <iKan/Renderer/Mesh.h>
#include <iKan/Renderer/Renderer.h>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

namespace iKan {

    glm::mat4 Mat4FromAssimpMat4(const aiMatrix4x4& matrix)
    {
        glm::mat4 result;
        //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
        result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
        result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
        result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
        result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
        return result;
    }

    static const uint32_t s_MeshImportFlags =
    aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
    aiProcess_Triangulate |             // Make sure we're triangles
    aiProcess_SortByPType |             // Split meshes by primitive type
    aiProcess_GenNormals |              // Make sure we have legit normals
    aiProcess_GenUVCoords |             // Convert UVs if required
    aiProcess_OptimizeMeshes |          // Batch draws where possible
    aiProcess_ValidateDataStructure;    // Validation
    
    struct LogStream : public Assimp::LogStream
    {
        static void Initialize()
        {
            if (Assimp::DefaultLogger::isNullLogger())
            {
                Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
                Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
            }
        }
        
        virtual void write(const char* message) override
        {
            IK_CORE_ERROR("Assimp error: {0}", message);
        }
    };
    
    Mesh::Mesh(const std::string& filename)
    : m_FilePath(filename)
    {
        LogStream::Initialize();
        
        IK_CORE_INFO("Loading mesh: {0}", filename.c_str());
        
        m_Importer = CreateScope<Assimp::Importer>();

        const aiScene* scene = m_Importer->ReadFile(filename, s_MeshImportFlags);
        if (!scene || !scene->HasMeshes())
            IK_CORE_ERROR("Failed to load mesh file: {0}", filename);
        
        m_Scene = scene;
        
        m_IsAnimated        = scene->mAnimations != nullptr;
        m_MeshShader        = m_IsAnimated ? Renderer::GetShaderLibrary()->Get("iKanPBR_Anim") : Renderer::GetShaderLibrary()->Get("iKanPBR_Static");
        m_BaseMaterial      = Ref<Material>::Create(m_MeshShader);
        m_InverseTransform  = glm::inverse(Mat4FromAssimpMat4(scene->mRootNode->mTransformation));
        
        uint32_t vertexCount = 0;
        uint32_t indexCount  = 0;
        
        m_Submeshes.reserve(scene->mNumMeshes);

    }
    
}
