#pragma once

#include <iKan/Renderer/GraphicsContext.h>

namespace iKan {
    
    static const uint32_t s_WindowWidth = 1280;
    static const uint32_t s_WindowHeight = 720;
    
    class Window
    {
    public:
        Window(const std::string& title = "ENGINE", uint32_t width = s_WindowWidth, uint32_t height = s_WindowHeight);
        ~Window();
        
        void Init();
        void Update();
        void Shutdown();
        
        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }
        
        const std::string& GetTitle() const { return m_Title; }
        
        GLFWwindow* GetNativeWindow() const { return m_Window; }
        
    private:
        void SetCallBacks();
        
    private:
        std::unique_ptr<GraphicsContext> m_Context;
        GLFWwindow* m_Window = nullptr;
        
        uint32_t m_Width = 0, m_Height = 0;
        std::string m_Title = "";
    };
    
}
