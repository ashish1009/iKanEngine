#pragma once

namespace iKan {
    
    class ImGuiAPI
    {
    public:
        static void RendererVersion();
        static void FrameRate();
        static void RendererStats();
        static void Counter(const std::string& name, uint32_t& counter);
        static void Counter(const std::string& name, float& counter);
        static void ColorEdit(glm::vec4& colorRef);
        static void EnableDcocking();
        static void EndDocking();
        static void LightGreyBackground();
    };
    
}
