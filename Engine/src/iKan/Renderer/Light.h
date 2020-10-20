#pragma once

namespace iKan {
    
    struct Light
    {
        struct Flag
        {
            bool IsAmbient      = true;
            bool IsDiffuse      = true;
            bool IsSpecular     = true;
            bool IsAttenuation  = true;
            bool IsSpotLight    = true;
        };
        
        Flag LightFlag;
        
        glm::vec3 Position = { 0.0f, 0.0f, -3.0f };
        glm::vec3 Ambient  = { 0.1f, 0.1f, 0.1f };
        glm::vec3 Diffuse  = { 0.8f, 0.8f, 0.8f };
        glm::vec3 Specular = { 1.0f, 1.0f, 1.0f };
        
        float Constant  = 1.0f;
        float Linear    = 0.09f;
        float Quadratic = 0.032f;
        float CutOff    = 12.5f; // radians
    };
    
}
