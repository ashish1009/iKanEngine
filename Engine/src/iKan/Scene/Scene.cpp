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
        glm::vec2 position = { currEntityPos[0], currEntityPos[1] };
    
        auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
        for (auto& entity : view)
        {
            auto [transform, tag, sprite] = m_Registry.get<TransformComponent, TagComponent, SpriteRendererComponent>(entity);
            glm::vec2 entitySpriteSize    = sprite.SubTexComp->GetSpriteSize();
            glm::vec2 entityPos           = { transform.Transform[3][0], transform.Transform[3][1] };
            
            if (currEntityTag != tag.Tag)
            {
                /* Up Down Collision*/
                float xCollDiff = 13.0f/16.0f + ((entitySpriteSize.x == 1.0f) ? 0.0f : 0.5f);
                if ((position.x + xCollDiff) >= entityPos.x && (position.x - xCollDiff) <= entityPos.x)
                    if((position.y - speed * 15.0f <= entityPos.y + 1) && (position.y - speed * 15.0f > entityPos.y))
                        result |= CollisionBit::Down;
                
                /* Right Left Collision */
                float yCollDiff  = 13.0f/16.0f;
                float nextEntityDiff = (entitySpriteSize.x == 1.0f) ? 1.0f : 1.5f;
                if ((position.y + yCollDiff) >= entityPos.y && (position.y - yCollDiff) <= entityPos.y)
                    if ((position.x + speed * 10.0f <= entityPos.x) && (position.x + speed * 10.0f > entityPos.x - nextEntityDiff))
                        result |= CollisionBit::Right;

            }
        }
        return result;
    }
    
}
