#pragma once

namespace iKan {

    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize,
        KeyPressed, KeyReleased, KeyType,
        MouseMove, MouseScroll, MouseButtonPressed, MouseButtonReleased,
    };
    
    class Event
    {
    public:        
        Event() = default;
        virtual ~Event() = default;
        
        virtual EventType GetType() const = 0;
        
    private:
    };
    
}
