#pragma once

#include <iKan/Core/Events/Events.h>
#include <iKan/Core/KeyCode.h>

namespace iKan {
    
    class KeyPressedEvent : public Event
    {
    public:
        KeyPressedEvent(KeyCode keyCode, int repeat)
        : m_KeyCode(keyCode), m_Count(repeat) {}
        
        virtual ~KeyPressedEvent() = default;
        
        KeyCode GetKeyCode() const { return m_KeyCode; }
        uint32_t GetKeyRepeat() const { return m_Count; }
        
        virtual EventType GetType() const override { return EventType::KeyPressed; }
        virtual int GetCategoryBit() const override { return EventCategory::KeyCategory; }
        
        static EventType GetStaticType() { return EventType::KeyPressed; }
        
    private:
        KeyCode  m_KeyCode;
        uint32_t m_Count;
    };
    
    class KeyReleasedEvent : public Event
    {
    public:
        KeyReleasedEvent(KeyCode keyCode)
        : m_KeyCode(keyCode) {}
        
        virtual ~KeyReleasedEvent() = default;
                
        KeyCode GetKeyCode() const { return m_KeyCode; }
        
        virtual EventType GetType() const override { return EventType::KeyReleased; }
        virtual int GetCategoryBit() const override { return EventCategory::KeyCategory; }
        
        static EventType GetStaticType() { return EventType::KeyReleased; }

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
        
        virtual EventType GetType() const override { return EventType::KeyType; }
        virtual int GetCategoryBit() const override { return EventCategory::KeyCategory; }
        
        static EventType GetStaticType() { return EventType::KeyType; }

    private:
        KeyCode m_KeyCode;
    };
    
}
