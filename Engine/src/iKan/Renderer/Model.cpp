#include <iKan/Renderer/Model.h>
#include <iKan/Renderer/Texture.h>

namespace iKan {
    
    Model::Model(const std::string& path)
    {
        LoadModel(path);
    }
    
    void Model::Draw(Shader &shader)
    {
        for(auto mesh : m_Meshes)
            mesh.Draw(shader);
    }
    
    void Model::LoadModel(const std::string &path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile( path,
                                                 aiProcess_Triangulate            |
                                                 aiProcess_JoinIdenticalVertices  |
                                                 aiProcess_GenNormals             |
                                                 aiProcess_OptimizeMeshes         |
                                                 aiProcess_SplitLargeMeshes);
        
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
            IK_CORE_ASSERT(false, importer.GetErrorString());
        
        m_Directory = path.substr(0, path.find_last_of('/'));
        ProcessNode(scene->mRootNode, scene);
    }
    
    void Model::ProcessNode(aiNode *node, const aiScene *scene)
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
    
    Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
    {
        std::vector<Mesh::MeshVertex>   vertices;
        std::vector<uint32_t>           indices;
        std::vector<Mesh::MeshTexture>  textures;
        
        for(uint32_t i = 0; i < mesh->mNumVertices; i++)
        {
            Mesh::MeshVertex vertex;
            
            glm::vec3 vector;
            
            /* positions */
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            
            /* normals */
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            
            /* texture coordinates */
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
                /* tangent */
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
            }
            
            if (mesh->mBitangents)
            {
                /* bitangent */
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
        
        /* 1. diffuse maps */
        std::vector<Mesh::MeshTexture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        
        /* 2. specular maps */
        std::vector<Mesh::MeshTexture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        
        /* 3. normal maps */
        std::vector<Mesh::MeshTexture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        
        /* 4. height maps */
        std::vector<Mesh::MeshTexture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        
        return Mesh(vertices, indices, textures);
    }
    
    std::vector<Mesh::MeshTexture> Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
    {
        std::vector<Mesh::MeshTexture> meshTextureVector;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
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
                
                Mesh::MeshTexture meshTexture;
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
