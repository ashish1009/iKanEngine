#pragma once

#include <iKan/Core/Core.h>

namespace iKan {

    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize,
        KeyPressed, KeyReleased, KeyType,
        MouseMove, MouseScroll, MouseButtonPressed, MouseButtonReleased,
    };
    
    enum EventCategory
    {
        None = 0,
        WindowCategory  = BIT(0),
        MouseCategory   = BIT(1),
        KeyCategory     = BIT(2)
    };
    
    class Event
    {
    public:        
        Event() = default;
        virtual ~Event() = default;
        
        virtual EventType GetType() const = 0;
        virtual int GetCategoryBit() const = 0;
        
        bool IsInCategory(EventCategory type) { return GetCategoryBit() & type; }
        
    public:
        bool Handled = false;
    };
    
}
