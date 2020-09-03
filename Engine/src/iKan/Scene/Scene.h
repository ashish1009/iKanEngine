#pragma once

#include <entt.hpp>
#include <iKan/Core/TimeStep.h>

#include <iKan/Scene/Components.h>

#include <iKan/Renderer/Renderer2D.h>

namespace iKan {
    
    class Entity;
    
    class Scene
    {
    public:
        Scene() = default;
        ~Scene() = default;
        
        Entity CreateEntity(const std::string& name = std::string());
        void OnViewportResize(uint32_t width, uint32_t height);

        void OnUpdate(TimeStep ts);
        
    private:
        /* Container that contain all the entities */
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        friend class Entity;
        
    };
    
}
