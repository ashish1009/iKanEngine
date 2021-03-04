// ******************************************************************************
//   File    : Window.h
//   Project : i-Kan : Core
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <iKan/Core/Events/Events.h>

namespace iKan {
    
    static const uint32_t s_WindowWidth = 1280;
    static const uint32_t s_WindowHeight = 720;
    
    // ******************************************************************************
    // Wrapper to create the property of Window
    // ******************************************************************************
    struct WindowProp
    {
        std::string Title;
        uint32_t Height, Width;
        
        WindowProp(std::string title = "iKan", uint32_t width = s_WindowWidth, uint32_t height = s_WindowHeight)
        : Title(title), Height(height), Width(width) { }
    };
    
    // ******************************************************************************
    // Interface for Creating window
    // ******************************************************************************
    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;
        
        virtual ~Window() = default;
        
        virtual void Update() = 0;
        virtual void Shutdown() = 0;
        
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        
        /* Window Attributes */
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;
        virtual void* GetNativeWindow() = 0;
        
        static Scope<Window> Create(const WindowProp& props = WindowProp());
    };
    
}
