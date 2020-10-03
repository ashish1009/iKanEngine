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

}
