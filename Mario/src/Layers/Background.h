#pragma once

#include <iKan.h>

namespace iKan {
    
    class Background
    {
    public:
        static void Init(const std::shared_ptr<Scene>& scene);
        static void ImGuiRender();
        static const glm::vec4& GetColor() { return s_BgColor; }
        
    private:
        static glm::vec4 s_BgColor;
    };
    
}
