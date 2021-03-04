// ******************************************************************************
//   File    : Bone.cpp
//   Project : i-Kan : Renderer
//
//   Created by Ashish
// ******************************************************************************

#include <iKan/Renderer/Graphics/Bone.h>
#include <iKan/Renderer/Graphics/Mesh.h>

namespace iKan {

    // ******************************************************************************
    // Constructing Bone
    // ******************************************************************************
    Bone::Bone(Mesh* inMesh, uint32_t inId, const std::string& inName, const aiMatrix4x4& inOMat)
    : ID(inId), Name(inName), OffsetMatrix(Bone::AiToGLMMat4(inOMat)), BoneMesh(inMesh)
    {
    }

    // ******************************************************************************
    // Constructing Bone
    // ******************************************************************************
    Bone::Bone(Mesh* inMesh, uint32_t inId, const std::string& inName, const glm::mat4& inOMat)
    : ID(inId), Name(inName), OffsetMatrix(inOMat), BoneMesh(inMesh)
    {
    }

    // ******************************************************************************
    // get Pattern
    // ******************************************************************************
    glm::mat4 Bone::GetParentTransforms()
    {
        Bone* b = ParentBone;
        std::vector<glm::mat4> mats;
        while(b != nullptr)
        {
            glm::mat4 tmpMat = AiToGLMMat4(b->Node->mTransformation);
            mats.push_back(tmpMat);

            b = b->ParentBone;
        }

        glm::mat4 concatenatedTransforms;
        for(size_t i = mats.size()-1; i >= 0; i--)
            concatenatedTransforms *= mats.at(i);

        return concatenatedTransforms;
    }

}
