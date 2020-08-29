#pragma once

#include <iKan/Events/Events.h>
#include <iKan/Core/KeyCode.h>

namespace iKan {
    
    class KeyPressedEvent : public Event
    {
    public:
        KeyPressedEvent(KeyCode keyCode, int repeat)
        : m_KeyCode(keyCode) {}
        
        virtual ~KeyPressedEvent() = default;
        
        virtual EventType GetType() const { return EventType::KeyPressed; }
        
        KeyCode GetKeyCode() const { return m_KeyCode; }
        
    private:
        KeyCode m_KeyCode;
    };
    
    class KeyReleasedEvent : public Event
    {
    public:
        KeyReleasedEvent(KeyCode keyCode)
        : m_KeyCode(keyCode) {}
        
        virtual ~KeyReleasedEvent() = default;
                
        KeyCode GetKeyCode() const { return m_KeyCode; }
        
        virtual EventType GetType() const { return EventType::KeyReleased; }
        
    private:
        KeyCode m_KeyCode;
    };
    
    class KeyTypeEvent : public Event
    {
    public:
        KeyTypeEvent(KeyCode keyCode)
        : m_KeyCode(keyCode) {}
        
        virtual ~KeyTypeEvent() = default;
        
        KeyCode GetKeyCode() const { return m_KeyCode; }
        
        virtual EventType GetType() const { return EventType::KeyType; }
        
    private:
        KeyCode m_KeyCode;
    };
    
}
