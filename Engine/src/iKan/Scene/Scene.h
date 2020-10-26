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
        ~Scene();
        
        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnViewportResize(uint32_t width, uint32_t height);\
        
        void OnRenderEditor(TimeStep ts, const EditorCamera& editorCamera);

        void OnUpdate(TimeStep ts);
        
    private:
        Entity GetMainCameraEntity();
        void InstantiateScripts(TimeStep ts);

        template<typename T>
        void OnComponentAdded(Entity entity, T& component);
        
    private:
        /* Container that contain all the entities */
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 1280.0f, m_ViewportHeight = 720.0f;
        
        friend class Entity;
        friend class SceneHeirarchyPannel;
        friend class SceneSerializer;

    };
    
}
