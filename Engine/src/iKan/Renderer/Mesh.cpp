#include <iKan/Renderer/Mesh.h>
#include <iKan/Renderer/Renderer.h>
#include <iKan/Renderer/RenderStats.h>

namespace iKan {
    
    static std::string GetStringFromAiTextureType(aiTextureType type)
    {
        switch (type)
        {
            case aiTextureType_NONE              : return "None";                   break;
            case aiTextureType_DIFFUSE           : return "Diffuse";                break;
            case aiTextureType_SPECULAR          : return "Specular";               break;
            case aiTextureType_AMBIENT           : return "Ambient";                break;
            case aiTextureType_EMISSIVE          : return "Emissive";               break;
            case aiTextureType_HEIGHT            : return "Height";                 break;
            case aiTextureType_NORMALS           : return "Normal";                 break;
            case aiTextureType_SHININESS         : return "Shininess";              break;
            case aiTextureType_OPACITY           : return "Opacity";                break;
            case aiTextureType_DISPLACEMENT      : return "Dispacement";            break;
            case aiTextureType_LIGHTMAP          : return "Lightmap";               break;
            case aiTextureType_REFLECTION        : return "Reflection";             break;
            case aiTextureType_BASE_COLOR        : return "BaseColor";              break;
            case aiTextureType_NORMAL_CAMERA     : return "NormalCamera";           break;
            case aiTextureType_EMISSION_COLOR    : return "EmissionColor";          break;
            case aiTextureType_METALNESS         : return "Metalness";              break;
            case aiTextureType_DIFFUSE_ROUGHNESS : return "DiffuseRoughness";       break;
            case aiTextureType_AMBIENT_OCCLUSION : return "AmbientOcclusion";       break;
            case aiTextureType_UNKNOWN           : return "Unknow";                 break;
            case _aiTextureType_Force32Bit       : return "MAX";                    break;
        }
    }
    
    SubMesh::SubMesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices, std::vector<MeshTexture> textures)
    : m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
    {
        setupMesh();
    }
    
    void SubMesh::Draw(Shader &shader)
    {
        uint32_t slot = 0;
        for(auto texture : m_Textures)
        {
            texture.Texture->Bind(slot);
            slot++;
            RendererStatistics::TextureCount++;
        }
        
        shader.Bind();
        Renderer::DrawIndexed(m_VAO);
        
        m_VAO->Unbind();
        shader.Unbind();
        
        RendererStatistics::VertexCount += m_Vertices.size();
        RendererStatistics::IndexCount  += m_Indices.size();
    }
    
    void SubMesh::setupMesh()
    {
        m_VAO = VertexArray::Create();
        
        Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(uint32_t(m_Vertices.size() * sizeof(MeshVertex)), (float*)(&m_Vertices[0]));
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
    
    // ----------------------------------------------------------- < Sub Mesh > -----------------------------------------------------------
    Mesh::Mesh(const std::string& path)
    {
        LoadModel(path);
    }
    
    void Mesh::Draw(Shader &shader) const 
    {
        for(auto mesh : m_Meshes)
            mesh.Draw(shader);
    }
    
    void Mesh::LoadModel(const std::string &path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile( path,
                                                 aiProcess_Triangulate            |
                                                 aiProcess_JoinIdenticalVertices  |
                                                 aiProcess_GenNormals             |
                                                 aiProcess_OptimizeMeshes         |
                                                 aiProcess_SplitLargeMeshes);
        
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            IK_CORE_ASSERT(false, importer.GetErrorString());
        
        m_Directory = path.substr(0, path.find_last_of('/'));
        ProcessNode(scene->mRootNode, scene);
    }
    
    void Mesh::ProcessNode(aiNode *node, const aiScene *scene)
    {
        for(uint32_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_Meshes.push_back(ProcessMesh(mesh, scene));
        }
        for(uint32_t i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }
    
    SubMesh Mesh::ProcessMesh(aiMesh *mesh, const aiScene *scene)
    {
        std::vector<MeshVertex>   vertices;
        std::vector<uint32_t>     indices;
        std::vector<MeshTexture>  textures;
        
        for(uint32_t i = 0; i < mesh->mNumVertices; i++)
        {
            MeshVertex vertex;
            
            glm::vec3 vector;
            
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            
            // normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            
            // texture coordinates
            if(mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
            {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }
            
            if (mesh->mTangents)
            {
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
            }
            
            if (mesh->mBitangents)
            {
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            
            vertices.push_back(vertex);
        }
        
        for(uint32_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(uint32_t j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        
        for (uint32_t i = aiTextureType_DIFFUSE; i < aiTextureType_UNKNOWN; i++)
        {
            std::vector<MeshTexture> maps = LoadMaterialTextures(material, aiTextureType(i), GetStringFromAiTextureType(aiTextureType(i)));
            textures.insert(textures.end(), maps.begin(), maps.end());
        }
        
        return SubMesh(vertices, indices, textures);
    }
    
    std::vector<MeshTexture> Mesh::LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
    {
        std::vector<MeshTexture> meshTextureVector;
        for(uint32_t i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            
            bool skip = false;
            for(auto textureLoaded : m_TexturesLoaded)
            {
                if(std::strcmp(textureLoaded.Path.data(), str.C_Str()) == 0)
                {
                    meshTextureVector.push_back(textureLoaded);
                    skip = true;
                    break;
                }
            }
            if(!skip)
            {
                std::string filePath = m_Directory + '/' + std::string(str.C_Str());
                
                MeshTexture meshTexture;
                meshTexture.Texture = Texture::Create(filePath);
                meshTexture.Type    = typeName;
                meshTexture.Path    = str.C_Str();
                
                meshTextureVector.push_back(meshTexture);
                
                m_TexturesLoaded.push_back(meshTexture);
            }
        }
        return meshTextureVector;
    }
}
