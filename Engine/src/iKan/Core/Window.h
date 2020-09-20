#pragma once

#include <iKan/Renderer/GraphicsContext.h>

#include <iKan/Core/Events/Events.h>

namespace iKan {
    
    static const uint32_t s_WindowWidth = 1280;
    static const uint32_t s_WindowHeight = 720;
    
    struct WindowProp
    {
        std::string Title;
        uint32_t Width;
        uint32_t Height;
        
        WindowProp(const std::string& title = "ENGINE", uint32_t width = s_WindowWidth, uint32_t height = s_WindowHeight)
        : Title(title), Width(width), Height(height) { }
    };
    
    class Window
    {
    public:
        Window(const WindowProp& prop = WindowProp());
        ~Window();
        
        void Init(const WindowProp& prop);
        void OnUpdate();
        void Shutdown();
        
        uint32_t GetWidth() const { return m_Data.Width; }
        uint32_t GetHeight() const { return m_Data.Height; }
        
        const std::string& GetTitle() const { return m_Data.Title; }
        GLFWwindow* GetNativeWindow() const { return m_Window; }
        void SetEventCallBack(std::function<void(Event&)> func) { m_Data.EventFunc = func; }
        
    private:
        void SetCallBacks();
        
    private:
        Scope<GraphicsContext> m_Context;
        GLFWwindow* m_Window = nullptr;
                
        // Encapsulate the Data that needs to be sent to glfw as pointer
        struct WindowData
        {
            uint32_t    Width = 0, Height = 0;
            std::string Title = "";
            
            std::function<void(Event&)> EventFunc;
        };
        
        WindowData m_Data;
    };
    
}
