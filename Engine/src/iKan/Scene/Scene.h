#pragma once

#include <entt.hpp>

#include <iKan/Core/TimeStep.h>
#include <iKan/Core/Core.h>

#include <iKan/Renderer/Renderer2D.h>

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
    
    class Scene : public RefCounted
    {
    public:
        struct Speeds
        {
            float Up = 0.0f, Down = 0.0f, Right = 0.0f, Left = 0.0f;
            
            Speeds(float up, float down, float right, float left)
            : Up(up), Down(down), Right(right), Left(left) {}
        };
        
    public:
        Scene() = default;
        ~Scene() = default;
        
        Entity CreateEntity(const std::string& name = std::string());
        void OnViewportResize(uint32_t width, uint32_t height);

        void OnUpdate(TimeStep ts);
        Collisions CollisionDetection(Entity& entity, Speeds speeds);
        
    private:
        /* Container that contain all the entities */
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        friend class Entity;
        
    };
    
}
