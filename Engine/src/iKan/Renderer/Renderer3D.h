#pragma once

#include <iKan/Renderer/Camera.h>

namespace iKan {
    
    class Renderer3D
    {
    public:
        static void Init();
        static void Shutdown();
        
        static void BeginScene(const Camera& camera, const glm::mat4& transform);
        
        static void EndScene();
        static void Flush();
        
        static void SetShaader(const std::string& path);
        
        /* Primitives */
        static void DrawCube(const glm::mat4& transform, const glm::vec4& color);
        
    private:
        static void FlushAndReset();
    };
    
}
