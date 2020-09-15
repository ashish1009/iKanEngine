#include <iKan/Scene/Scene.h>
#include <iKan/Scene/Components.h>
#include <iKan/Scene/Entity.h>

#include <iKan/Renderer/Renderer2D.h>

namespace iKan {
    
    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TransformComponent>();
        
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag   = name.empty() ? "Entity" : name;
        
        return entity;
    }
    
    void Scene::OnUpdate(TimeStep ts)
    {
        m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
        {
            for (auto script : nsc.Scripts)
            {
                if (!script->m_Created)
                {
                    script->m_Entity = { entity, this };
                    script->OnCreate();
                }
                script->OnUpdate(ts);
            }
        });
        
        // Render 2D Sprites
        Camera* mainCamera         = nullptr;
        glm::mat4* cameraTransform = nullptr;
        
        auto view = m_Registry.view<TransformComponent, CameraComponent>();
        for (auto entity : view)
        {
            const auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
            if (camera.Primary)
            {
                mainCamera      = &camera.Camera;
                cameraTransform = &transform.Transform;
                break;
            }
        }
        
        if (mainCamera)
        {
            Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);
            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group)
            {
                const auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                if (sprite.SubTexComp)
                    Renderer2D::DrawQuad(transform, sprite.SubTexComp);
                else
                    Renderer2D::DrawQuad(transform, sprite.Color);
            }
            Renderer2D::EndScene();
        }
    }
    
    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth  = width;
        m_ViewportHeight = height;
        
        // Resize our non-FixedAspectRatio cameras
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio)
                cameraComponent.Camera.SetViewportSize(width, height);
        }
        
    }
        
    Collisions Scene::CollisionDetection(Entity& currEntity, float speed)
    {
        Collisions result = CollisionBit::NoCollision;
        auto currEntityTag = currEntity.GetComponent<TagComponent>().Tag;
        auto currEntityPos = currEntity.GetComponent<TransformComponent>().Transform[3];
        auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
        for (auto& entity : view)
        {
            auto [transform, tag, sprite] = m_Registry.get<TransformComponent, TagComponent, SpriteRendererComponent>(entity);
            auto entityPos = transform.Transform[3];
            if (currEntityTag != tag.Tag)
            {
                // Using floor here as y Position can be negative too
                /* Up Down Collision*/
                if (int32_t(currEntityPos[0]) == int32_t(entityPos[0]))
                {
                    if(IK_FLOOR(currEntityPos[1] - (speed * 15.0f)) == IK_FLOOR(entityPos[1]))
                        result |= CollisionBit::Down;
                }
                
                /* Right Left Collision */
                if (IK_FLOOR(currEntityPos[1]) == IK_FLOOR(entityPos[1]))
                {
                    auto spriteSize      = sprite.SubTexComp->GetSpriteSize();
                    float nextEntityDiff = (spriteSize.x == 1.0f) ? 1.0f : 1.5f;
                    if (int32_t(currEntityPos[0] + nextEntityDiff + (10.0f * speed)) == int32_t(entityPos[0]))
                        result |= CollisionBit::Right;
                }

            }
        }
        return result;
    }
    
}
