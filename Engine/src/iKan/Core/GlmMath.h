// ******************************************************************************
//   File    : GlmMath.h
//   Project : i-Kan : Core
//
//   Created by Ashish
// ******************************************************************************

#pragma once

namespace iKan {
    
    // ******************************************************************************
    // Some Wrapper to modify GLM Functionality
    // ******************************************************************************
    class GlmMath
    {
    public:
        static std::tuple<glm::vec3, glm::quat, glm::vec3> GetTransformDecomposition(const glm::mat4& transform);
        static glm::mat4 SetTransfrom(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    };
    
}
