// ******************************************************************************
//   File    : MouseEvents.h
//   Project : i-Kan : Core
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <iKan/Core/Events/Events.h>
#include <iKan/Core/Input.h>

namespace iKan {
    
    // ******************************************************************************
    // Mouse base Event
    // ******************************************************************************
    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float x, float y)
        : m_MouseX(x), m_MouseY(y) {}
        
        float GetX() const { return m_MouseX; }
        float GetY() const { return m_MouseY; }
        
        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float m_MouseX, m_MouseY;
    };
    
    // ******************************************************************************
    // mouse scrol event
    // ******************************************************************************
    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(float xOffset, float yOffset)
        : m_XOffset(xOffset), m_YOffset(yOffset) {}
        
        float GetXOffset() const { return m_XOffset; }
        float GetYOffset() const { return m_YOffset; }
        
        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float m_XOffset, m_YOffset;
    };
    
    // ******************************************************************************
    // mouse button base calss
    // ******************************************************************************
    class  MouseButtonEvent : public Event
    {
    public:
        MouseCode GetMouseButton() const { return m_Button; }
        
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
    protected:
        MouseButtonEvent(MouseCode button)
        : m_Button(button) {}
        
        MouseCode m_Button;
    };
    
    // ******************************************************************************
    // Mouse button pressed
    // ******************************************************************************
    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(MouseCode button)
        : MouseButtonEvent(button) {}
        
        EVENT_CLASS_TYPE(MouseButtonPressed)
    };
    
    // ******************************************************************************
    // Mouse button released
    // ******************************************************************************
    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(MouseCode button)
        : MouseButtonEvent(button) {}
                
        EVENT_CLASS_TYPE(MouseButtonReleased)
    };
    
}
