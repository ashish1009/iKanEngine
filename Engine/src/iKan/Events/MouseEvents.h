#pragma once

#include <iKan/Events/Events.h>
#include <iKan/Core/MouseCode.h>

namespace iKan {
    
    class MouseMoveEvent : public Event
    {
    public:
        MouseMoveEvent(int xPos, int yPos)
        : m_XPos(xPos), m_YPos(yPos) {}
        
        virtual ~MouseMoveEvent() = default;
        
        int GetXPos() const { return m_XPos; }
        int GetYPos() const { return m_YPos; }
        
        virtual EventType GetType() const { return EventType::MouseMove; }
        
    private:
        int m_XPos = 0, m_YPos = 0;
    };
    
    class MouseScrollEvent : public Event
    {
    public:
        MouseScrollEvent(int xOffset, int yOffset)
        : m_XOffset(xOffset), m_YOffset(yOffset) {}
        
        virtual ~MouseScrollEvent() = default;
        
        int GetXOffset() const { return m_XOffset; }
        int GetYOffset() const { return m_YOffset; }
        
        virtual EventType GetType() const { return EventType::MouseScroll; }
        
    private:
        int m_XOffset = 0, m_YOffset = 0;
    };
    
    class MouseButtonPressEvent : public Event
    {
    public:
        MouseButtonPressEvent(MouseCode buttonCode)
        : m_ButtonCode(buttonCode) {}
        
        virtual ~MouseButtonPressEvent() = default;
        
        MouseCode GetButtonCode() const { return m_ButtonCode; }
        
        virtual EventType GetType() const { return EventType::MouseButtonPressed; }
        
    private:
        MouseCode m_ButtonCode;
    };
    
    class MouseReleasePressEvent : public Event
    {
    public:
        MouseReleasePressEvent(MouseCode buttonCode)
        : m_ButtonCode(buttonCode) {}
        
        virtual ~MouseReleasePressEvent() = default;
        
        MouseCode GetButtonCode() const { return m_ButtonCode; }
        
        virtual EventType GetType() const { return EventType::MouseButtonReleased; }
        
    private:
        MouseCode m_ButtonCode;
    };
}
