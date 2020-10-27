#pragma once

namespace iKan {
    
    struct SceneLight
    {
    public:
        bool IsAmbient = false, IsDiffuse = false, IsSpecular = false;
                
        glm::vec3 Ambient  = { 0.2f, 0.2f, 0.2f };
        glm::vec3 Diffuse  = { 0.5f, 0.5f, 0.5f };
        glm::vec3 Specular = { 1.0f, 1.0f, 1.0f };
    };
    
}
