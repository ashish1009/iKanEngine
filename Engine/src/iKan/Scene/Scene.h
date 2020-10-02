#pragma once

#include <entt.hpp>

#include <iKan/Core/TimeStep.h>
#include <iKan/Core/Core.h>

#include <iKan/Renderer/Renderer2D.h>

#include <iKan/Editor/EditorCamera.h>

namespace iKan {
    
    typedef int Collisions;
    enum CollisionBit
    {
        NoCollision = 0,
        Up          = BIT(0),
        Down        = BIT(1),
        Right       = BIT(2),
        Left        = BIT(3)
    };

    class Entity;
    
    struct Light
    {
        struct Flag
        {
            bool IsAmbient  = false;
            bool IsDiffuse  = false;
            bool IsSpecular = false;
        };
        
        Flag LightFlag;
        
        glm::vec3 Position = { 0.0f, 0.0f, -3.0f };
        glm::vec3 Ambient  = { 0.2f, 0.2f, 0.2f };
        glm::vec3 Diffuse  = { 0.5f, 0.5f, 0.5f };
        glm::vec3 Specular = { 1.0f, 1.0f, 1.0f };
    };
    
    class Scene : public RefCounted
    {
    public:
        enum class SceneRendererType { _2D, _3D };
        
        struct Speeds
        {
            float Up = 0.0f, Down = 0.0f, Right = 0.0f, Left = 0.0f;
            
            Speeds(float up, float down, float right, float left)
            : Up(up), Down(down), Right(right), Left(left) {}
        };
        
    public:
        Scene(SceneRendererType sceneRenderer);
        ~Scene() = default;
        
        Entity CreateEntity(const std::string& name = std::string());
        void OnViewportResize(uint32_t width, uint32_t height);

        void OnUpdate(TimeStep ts);
        void OnEditorUpdate(TimeStep ts, const EditorCamera& camera);
        Collisions CollisionDetection(Entity& entity, Speeds speeds);
        SceneRendererType GetRendererType() const { return m_ScceneRenderer; }
        Light& GetLight() { return m_Light; }
        const Light& GetLight() const { return m_Light; }
        
    private:
        Entity GetMainCameraEntity();
        void InstantiateScripts(TimeStep ts);
        
        void Renderer2D();
        void Renderer3D();
        
    private:
        /* Container that contain all the entities */
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
        
        Camera* m_MainCamera         = nullptr;
        glm::mat4* m_CameraTransform = nullptr;
        
        SceneRendererType m_ScceneRenderer;
        Light m_Light;

        friend class Entity;
        friend class SceneHeirarchyPannel;

    };
    
}
