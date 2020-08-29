#pragma once

#include <iKan/Events/Events.h>

namespace iKan {
    
    class KeyPressedEvent : public Event
    {
    public:
        KeyPressedEvent(int keyCode, int repeat)
        : m_KeyCode(keyCode) {}
        
        virtual ~KeyPressedEvent() = default;
        
        virtual EventType GetType() const { return EventType::KeyPressed; }
        
        int GetKeyCode() const { return m_KeyCode; }
        
    private:
        int m_KeyCode;
    };
    
    class KeyReleasedEvent : public Event
    {
    public:
        KeyReleasedEvent(int keyCode)
        : m_KeyCode(keyCode) {}
        
        virtual ~KeyReleasedEvent() = default;
                
        int GetKeyCode() const { return m_KeyCode; }
        
        virtual EventType GetType() const { return EventType::KeyReleased; }
        
    private:
        int m_KeyCode;
    };
    
    class KeyTypeEvent : public Event
    {
    public:
        KeyTypeEvent(int keyCode)
        : m_KeyCode(keyCode) {}
        
        virtual ~KeyTypeEvent() = default;
        
        int GetKeyCode() const { return m_KeyCode; }
        
        virtual EventType GetType() const { return EventType::KeyType; }
        
    private:
        int m_KeyCode;
    };
    
}
