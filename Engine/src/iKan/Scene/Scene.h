#pragma once

#include <entt.hpp>

#include <iKan/Core/TimeStep.h>
#include <iKan/Core/Core.h>
#include <iKan/Renderer/API/Renderer2D.h>
#include <iKan/Renderer/Graphics/FrameBuffer.h>
#include <iKan/Editor/EditorCamera.h>
#include <iKan/Scene/SceneLight.h>
#include <iKan/Core/UUID.h>

namespace iKan {
    
    class Entity;
    using EntityMap = std::unordered_map<UUID, Entity>;

    class Scene : public RefCounted
    {
    public:
        enum class CollisionSide
        {
            Up    = BIT(0),
            Down  = BIT(1),
            Right = BIT(2),
            Left  = BIT(3),
        };
        
    public:
        Scene();
        ~Scene();
        
        Entity CreateEntity(const std::string& name = std::string());
        Entity CreateEntityWithID(UUID uuid, const std::string& name = "");

        void DestroyEntity(Entity entity);

        void OnViewportResize(uint32_t width, uint32_t height);

        void OnUpdateRuntime(TimeStep ts);
        void OnUpdateEditor(TimeStep ts, EditorCamera& camera);

        const EntityMap& GetEntityMap() const { return m_EntityIDMap; }
        
        int BoxCollisionDetection(Entity& entity, float speed);
        
        Entity GetMainCameraEntity();
        Entity GetLightEntity();

        int32_t Pixel(int mx, int my);

    private:
        void InstantiateScripts(TimeStep ts);

        template<typename T>
        void OnComponentAdded(Entity entity, T& component);
        
    private:
        /* Container that contain all the entities */
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 1280.0f, m_ViewportHeight = 720.0f;
        
        EntityMap m_EntityIDMap;

        friend class Entity;
        friend class SceneHeirarchyPannel;
        friend class SceneSerializer;

    };
    
}
