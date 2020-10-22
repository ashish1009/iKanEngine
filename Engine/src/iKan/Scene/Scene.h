#pragma once

#include <entt.hpp>

#include <iKan/Core/TimeStep.h>
#include <iKan/Core/Core.h>

#include <iKan/Renderer/API/Renderer2D.h>

#include <iKan/Editor/EditorCamera.h>

namespace iKan {
    
    class Entity;
    
    class Scene : public RefCounted
    {
    public:
        Scene();
        ~Scene() = default;
        
        Entity CreateEntity(const std::string& name = std::string());
        void OnViewportResize(uint32_t width, uint32_t height);

        void OnUpdate(TimeStep ts);
        
    private:
        Entity GetMainCameraEntity();
        void InstantiateScripts(TimeStep ts);
        
    private:
        /* Container that contain all the entities */
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
        
        friend class Entity;
        friend class SceneHeirarchyPannel;

    };
    
}
