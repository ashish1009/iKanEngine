#include <iKan/Core/GlmMath.h>

namespace iKan {
    
    std::tuple<glm::vec3, glm::quat, glm::vec3> GlmMath::GetTransformDecomposition(const glm::mat4& transform)
    {
        glm::vec3 scale, translation, skew;
        glm::vec4 perspective;
        glm::quat orientation;
        glm::decompose(transform, scale, orientation, translation, skew, perspective);
        
        // TODO: improve api for perspective and skew
        return { translation, orientation, scale };
    }
    
    glm::mat4 GlmMath::SetTransfrom(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
    {
        return glm::translate(glm::mat4(1.0f), position) * glm::toMat4(glm::quat(glm::radians(rotation))) * glm::scale(glm::mat4(1.0f), scale);
    }

}
