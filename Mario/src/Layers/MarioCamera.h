#pragma once

#include <iKan.h>

namespace iKan {
    
    class MarioCamera
    {
    public:
        static void Init(const std::shared_ptr<Scene>& scene);
        static void ImGuiRender();
    };
    
}
