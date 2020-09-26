#pragma once

#include <iKan/Renderer/Camera.h>

namespace iKan {
    
    class SceneRenderer
    {
    public:
        static void Init();
        
        static void BeginScene(const Camera& camera, const glm::mat4& transform);
        
        static void DrawCube();
        
        static void EndScene();
        
        static void Shutdown();
    };
    
}
