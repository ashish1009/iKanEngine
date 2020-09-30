#pragma once

#include <iKan/Renderer/Camera.h>

namespace iKan {

    struct SceneRendererCamera
    {
        Camera    Camera;
        glm::mat4 ViewMatrix;
    };

    class SceneRenderer
    {
    public:
        static void Init();
        static void Shutdown();
        
        static void BeginScene(const SceneRendererCamera& camera);
        
        static void Draw();
        
        static void EndScene();
        static void Flush();
        
        static void SetShaader(const std::string& path);
            
    private:
        static void FlushAndReset();
    };
    
}
