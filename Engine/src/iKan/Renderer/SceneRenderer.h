#pragma once

#include <iKan/Renderer/Camera.h>

namespace iKan {
    
    class SceneRenderer
    {
    public:
        static void Init();
        static void Shutdown();
        
        static void BeginScene(const Camera& camera, const glm::mat4& transform);
        
        static void EndScene();
        static void Flush();
        
        static void SetShaader(const std::string& path);
            
    private:
        static void FlushAndReset();
    };
    
}
