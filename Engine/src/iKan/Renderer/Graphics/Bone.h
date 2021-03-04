// ******************************************************************************
//   File    : Bone.h
//   Project : i-Kan : Renderer
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <assimp/scene.h>

namespace iKan {

    class Mesh;
    class Skeleton;

    // ******************************************************************************
    // Bone CLaass
    // ******************************************************************************
    class Bone
    {
    public:
        uint32_t    ID;
        std::string Name;
        Mesh*       BoneMesh;
        glm::mat4   ParentTransforms;
        glm::mat4   OffsetMatrix;
        Skeleton*   ParentSkeleton;
        aiNodeAnim* AnimNode;
        Bone*       ParentBone = nullptr;
        aiNode*     Node = nullptr;

        Bone()
        {
            Name = "";
            ID   = -2;
        }

        Bone(Mesh* inMesh, uint32_t inId, const std::string& inName, const aiMatrix4x4& inOMat);
        Bone(Mesh* inMesh, uint32_t inId, const std::string& inName, const glm::mat4& inOMat);

        glm::mat4 GetParentTransforms();

#ifdef NOT_IN_USE
        // ******************************************************************************
        // Convert the glm mat to aiMAT
        // ******************************************************************************
        static aiMatrix4x4 GLMMat4ToAi(const glm::mat4& mat)
        {
            return aiMatrix4x4(mat[0][0],mat[0][1],mat[0][2],mat[0][3],
                               mat[1][0],mat[1][1],mat[1][2],mat[1][3],
                               mat[2][0],mat[2][1],mat[2][2],mat[2][3],
                               mat[3][0],mat[3][1],mat[3][2],mat[3][3]);
        }
#endif
        // ******************************************************************************
        // Convert the aiMAT to glm mat
        // ******************************************************************************
        static glm::mat4 AiToGLMMat4(const aiMatrix4x4& inMat)
        {
            glm::mat4 tmp;
            tmp[0][0] = inMat.a1;
            tmp[1][0] = inMat.b1;
            tmp[2][0] = inMat.c1;
            tmp[3][0] = inMat.d1;

            tmp[0][1] = inMat.a2;
            tmp[1][1] = inMat.b2;
            tmp[2][1] = inMat.c2;
            tmp[3][1] = inMat.d2;

            tmp[0][2] = inMat.a3;
            tmp[1][2] = inMat.b3;
            tmp[2][2] = inMat.c3;
            tmp[3][2] = inMat.d3;

            tmp[0][3] = inMat.a4;
            tmp[1][3] = inMat.b4;
            tmp[2][3] = inMat.c4;
            tmp[3][3] = inMat.d4;
            return tmp;
        }
    };

}
