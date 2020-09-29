#pragma once

namespace iKan {
    
    enum class ImGuiRendererType { _2D, _3D };
    
    class ImGuiAPI
    {
    public:
        static void RendererVersion();
        static void StatsAndFrameRate(ImGuiRendererType type);
        static void Counter(const std::string& name, uint32_t& counter);
        static void Counter(const std::string& name, float& counter);
        static void ColorEdit(glm::vec4& colorRef);
        static void EnableDcocking();
        static void EndDocking();
        static void LightGreyBackground();
    };
    
}
