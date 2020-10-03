#pragma once

namespace iKan {
    
    class Math
    {
    public:
        static std::tuple<glm::vec3, glm::quat, glm::vec3> GetTransformDecomposition(const glm::mat4& transform);
    };
    
}
