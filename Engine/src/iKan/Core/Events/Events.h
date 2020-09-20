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
    
    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event)
        : m_Event(event) { }
        
        /* F will be deduced by the compiler */
        template<typename T, typename F>
        void Dispatch(const F& func)
        {
            if (m_Event.GetType() == T::GetStaticType())
                m_Event.Handled = func(static_cast<T&>(m_Event));
        }
        
    private:
        Event& m_Event;
    };
    
}
