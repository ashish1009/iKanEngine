#pragma once

#include <iKan/Scene/Entity.h>

namespace iKan {
    
    class ScriptableEntity
    {
    public:
        virtual ~ScriptableEntity() = default;
        
        template<typename T>
        bool HasComponent()
        {
            return m_Entity.HasComponent<T>();
        }
        
        template<typename T>
        T& GetComponent()
        {
            return m_Entity.GetComponent<T>();
        }
        
    protected:
        virtual void OnCreate() { m_Created = true; }
        virtual void OnUpdate(TimeStep ts) {}
        virtual void OnDestroy() {}
        
    protected:
        bool m_Created = false;
        Entity m_Entity;
        
        friend class Scene;
    };
    
}
