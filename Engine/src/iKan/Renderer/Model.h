#pragma once

#include <iKan/Renderer/Mesh.h>

namespace iKan {
    
    class Model
    {
    public:
        Model(const std::string& path);
        ~Model() = default;
        
        void Draw(Shader &shader);
        
    private:
        void LoadModel(const std::string& path);
        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Mesh::MeshTexture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
        
    private:
        std::vector<Mesh>           m_Meshes;
        std::string                 m_Directory;
        std::vector<Mesh::MeshTexture>  m_TexturesLoaded;
    };
    
}
