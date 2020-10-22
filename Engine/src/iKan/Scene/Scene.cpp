#include <iKan/Scene/Scene.h>
#include <iKan/Scene/Components.h>
#include <iKan/Scene/Entity.h>

#include <iKan/Renderer/API/Renderer2D.h>

namespace iKan {
    
    Scene::Scene()
    {
    }
    
    Entity Scene::CreateEntity(const std::string& name)
    {
        // Creating the Entity
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TransformComponent>();
        
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag   = name.empty() ? "Entity" : name;
        
        return entity;
    }
    
    
    void Scene::OnUpdate(TimeStep ts)
    {
        // For all Entity having Native Scripts just instantiate the Scrips Binded to them and update them
        InstantiateScripts(ts);
        
        Camera* mainCamera         = nullptr;
        glm::mat4* cameraTransform = nullptr;
        
        if (Entity cameraEntity = GetMainCameraEntity();
            cameraEntity!= Entity(entt::null, this))
        {
            mainCamera      = &cameraEntity.GetComponent<CameraComponent>().Camera;
            cameraTransform = &cameraEntity.GetComponent<TransformComponent>().Transform;
        }
        
        // Renderer
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
        else
        {
            IK_CORE_WARN("No Camera is Binded to the Scene or any one of themis not set to primary !!! ");
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
     
    Entity Scene::GetMainCameraEntity()
    {
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& comp = view.get<CameraComponent>(entity);
            if (comp.Primary)
                return { entity, this };
        }
        return {};
    }
    
    void Scene::InstantiateScripts(TimeStep ts)
    {
        m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
                                                      {
            // nsc.Scripts is the Vector to store multiple Scripts for 1 entity
            for (auto script : nsc.Scripts)
            {
                // If a script is not created before then create the script and update the function
                if (!script->m_Created)
                {
                    script->m_Entity = { entity, this };
                    script->OnCreate();
                }
                script->OnUpdate(ts);
            }
        });

    }
    
}
