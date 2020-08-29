#pragma once

namespace iKan {

    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize,
    };
    
    class Event
    {
    public:        
        Event() = default;
        virtual ~Event() = default;
        
        virtual EventType GetType() = 0;
        
    private:
    };
    
}
