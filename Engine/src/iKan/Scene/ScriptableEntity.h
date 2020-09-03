#pragma once

#include <iKan/Scene/Entity.h>

namespace iKan {
    
    class ScriptableEntity
    {
    public:
        virtual ~ScriptableEntity() = default;

        template<typename T>
        T& GetComponent()
        {
            return m_Entity.GetComponent<T>();
        }
        
    protected:
        virtual void OnCreate() {}
        virtual void OnUpdate(TimeStep ts) {}
        virtual void OnDestroy() {}
        
    private:
        Entity m_Entity;
        
        friend class Scene;
    };
    
}
