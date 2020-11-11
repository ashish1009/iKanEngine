#pragma once

namespace iKan {
    
    class ImGuiAPI
    {
    public:
        static void RendererVersion(bool *pIsOpen = nullptr);
        static void FrameRate(bool *pIsOpen = nullptr);
        static void RendererStats(bool *pIsOpen = nullptr);
        static void Counter(const std::string& name, uint32_t& counter);
        static void Counter(const std::string& name, float& counter);
        static void ColorEdit(glm::vec4& colorRef);
        static void EnableDcocking();
        static void EndDocking();
        static void SetGreyThemeColors();
        static void SetLightThemeColors();
        static void SetDarkThemeColors();
    };
    
}
