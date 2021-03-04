// ******************************************************************************
//   File    : KeyEvents.h
//   Project : i-Kan : Core
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <iKan/Core/Events/Events.h>
#include <iKan/Core/Input.h>

namespace iKan {
    
    // ******************************************************************************
    // Key Base event
    // ******************************************************************************
    class KeyEvent : public Event
    {
    public:
        KeyCode GetKeyCode() const { return m_KeyCode; }
        
        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        KeyEvent(KeyCode keycode)
        : m_KeyCode(keycode) {}
        
        KeyCode m_KeyCode;
    };
    
    // ******************************************************************************
    // Key Pressed Event
    // ******************************************************************************
    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(KeyCode keycode, int32_t repeatCount)
        : KeyEvent(keycode), m_RepeatCount(repeatCount) {}
        
        int32_t GetRepeatCount() const { return m_RepeatCount; }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int32_t m_RepeatCount;
    };
    
    // ******************************************************************************
    // Key Released event
    // ******************************************************************************
    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(KeyCode keycode)
        : KeyEvent(keycode) {}

        EVENT_CLASS_TYPE(KeyReleased)
    };
    
    // ******************************************************************************
    // Key typed
    // ******************************************************************************
    class KeyTypedEvent : public KeyEvent
    {
    public:
        KeyTypedEvent(KeyCode keycode)
        : KeyEvent(keycode) {}        
        EVENT_CLASS_TYPE(KeyTyped)
    };
}
