// ******************************************************************************
//   File    : MacWindow.h
//   Project : i-Kan : Platform
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <iKan/Core/Window.h>
#include <iKan/Renderer/Graphics/GraphicsContext.h>

namespace iKan {
    
    // ******************************************************************************
    // Implementation of Mac Window
    // ******************************************************************************
    class MacWindow : public Window
    {
    public:
        MacWindow(const WindowProp& prop);
        virtual ~MacWindow();
        
        virtual void Update() override;
        virtual uint32_t GetWidth()  const override { return m_Data.Width; }
        virtual uint32_t GetHeight() const override { return m_Data.Height; }
        virtual void Shutdown() override;
        
        // Window attributes
        virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        virtual void SetVSync(bool enabled) override;
        virtual bool IsVSync() const override;
        virtual void* GetNativeWindow() override { return (void*)m_Window; }
        
    private:
        void Init(const WindowProp& prop);
        void SetCallBacks();

    private:
        Scope<GraphicsContext> m_Context;
        GLFWwindow* m_Window;
        
        // ******************************************************************************
        // Wrapper to store MAC Window data
        // ******************************************************************************
        struct WindowData
        {
            std::string Title;
            uint32_t    Width, Height;
            bool        VSync;
            
            EventCallbackFn EventCallback;
        };
        
        WindowData m_Data;
    };
    
    
}
