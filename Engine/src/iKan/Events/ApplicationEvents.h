#pragma once

#include <iKan/Events/Events.h>

namespace iKan {
    
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(uint32_t width, uint32_t height)
        : m_Width(width), m_Height(height) { }
        
        virtual ~WindowResizeEvent() = default;
        
        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }
        
        EventType GetType() { return EventType::WindowResize; }
        
    private:
        uint32_t m_Width = 0, m_Height = 0;
    };
    
    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;
        virtual ~WindowCloseEvent() = default;
        
        EventType GetType() { return EventType::WindowClose; }
    };
    
}
