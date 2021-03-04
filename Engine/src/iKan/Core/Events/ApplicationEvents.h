// ******************************************************************************
//   File    : ApplicationEvents.h
//   Project : i-Kan : Core
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <iKan/Core/Events/Events.h>

namespace iKan {
    
    // ******************************************************************************
    // Window Resize event
    // ******************************************************************************
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(uint32_t width, unsigned int height)
        : m_Width(width), m_Height(height) {}
        
        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    private:
        uint32_t m_Width, m_Height;
    };
    
    // ******************************************************************************
    // Window Close event
    // ******************************************************************************
    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;
        
        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
    
}
