#pragma once

#include <iKan.h>

namespace iKan {
    
    class CameraController;
    
    class MarioCamera
    {
    public:
        static void Init(const std::shared_ptr<Scene>& scene);
        static void ImGuiRender();
        
    private:
        static float s_Speed;
        
        friend class CameraController;
    };
    
}
