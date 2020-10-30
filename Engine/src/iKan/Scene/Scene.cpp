#include <iKan/Scene/Scene.h>
#include <iKan/Scene/Components.h>
#include <iKan/Scene/Entity.h>

#include <iKan/Renderer/API/Renderer2D.h>
#include <iKan/Renderer/API/SceneRenderer.h>

namespace iKan {
    
    Scene::Scene()
    {
        IK_CORE_INFO("Creating Scene !!");
    }
    
    Scene::~Scene()
    {
        IK_CORE_INFO("Destroying Scene !!");
    }
    
    Entity Scene::CreateEntity(const std::string& name)
    {
        // Creating the Entity
        Entity entity = { m_Registry.create(), this };

        auto& idComponent = entity.AddComponent<IDComponent>();
        idComponent.ID = {};

        entity.AddComponent<TransformComponent>();
        
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag   = name.empty() ? "Entity" : name;
        
        m_EntityIDMap[idComponent.ID] = entity;
        
        return entity;
    }
    
    Entity Scene::CreateEntityWithID(UUID uuid, const std::string& name)
    {
        auto entity = Entity{ m_Registry.create(), this };
        auto& idComponent = entity.AddComponent<IDComponent>();
        idComponent.ID = uuid;
        
        entity.AddComponent<TransformComponent>();
        if (!name.empty())
            entity.AddComponent<TagComponent>(name);
        
        IK_CORE_ASSERT((m_EntityIDMap.find(uuid) == m_EntityIDMap.end()), "Entity Akready Added");
        m_EntityIDMap[uuid] = entity;
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        m_Registry.destroy(entity);
    }
    
    void Scene::OnRenderEditor(TimeStep ts, const EditorCamera &editorCamera)
    {
        auto lightView = m_Registry.view<LightComponent>();
        for (auto entity : lightView)
        {
            SceneLight* light = nullptr;
            glm::vec3 lightPosition;
            auto& comp = lightView.get<LightComponent>(entity);
            if (comp.IsLight)
            {
                Entity lightEnt = { entity, this };
                light = &lightEnt.GetComponent<LightComponent>().Light;
                lightPosition = lightEnt.GetComponent<TransformComponent>().Translation;
                
                SceneRenderer::SetupLight({ light, lightPosition, editorCamera.GetPosition(), editorCamera.GetForwardDirection() });
            }
        }
        
        SceneRenderer::BegineScene({ editorCamera, editorCamera.GetPosition(), editorCamera.GetViewMatrix(), editorCamera.GetForwardDirection() });
        auto meshGroup = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
        for (auto entity : meshGroup)
        {
            const auto [transform, mesh] = meshGroup.get<TransformComponent, MeshComponent>(entity);
            if (mesh.Mesh)
                SceneRenderer::Draw(mesh.Mesh, transform.GetTransform());
        }
        SceneRenderer::EndScene();
        
        Renderer2D::BeginScene(editorCamera, glm::inverse(editorCamera.GetViewMatrix()));
        auto spriteGroup = m_Registry.group<>(entt::get<TransformComponent, SpriteRendererComponent>);
        for (auto entity : spriteGroup)
        {
            const auto [transform, sprite] = spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);
            if (sprite.Texture)
                Renderer2D::DrawQuad(transform.GetTransform(), sprite.Texture, sprite.TilingFactor, sprite.Color);
            else if (sprite.SubTexComp)
                Renderer2D::DrawQuad(transform.GetTransform(), sprite.SubTexComp, sprite.TilingFactor, sprite.Color);
            else
                Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
        }
        Renderer2D::EndScene();
    }
    
    void Scene::OnUpdate(TimeStep ts)
    {
        // For all Entity having Native Scripts just instantiate the Scrips Binded to them and update them
        InstantiateScripts(ts);
        
        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        if (Entity cameraEntity = GetMainCameraEntity();
            cameraEntity!= Entity(entt::null, nullptr))
        {
            mainCamera      = &cameraEntity.GetComponent<CameraComponent>().Camera;
            cameraTransform = cameraEntity.GetComponent<TransformComponent>().GetTransform();
        }
        
        // Renderer
        if (mainCamera)
        {
            Renderer2D::BeginScene(*mainCamera, cameraTransform);
            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group)
            {
                const auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                if (sprite.Texture)
                    Renderer2D::DrawQuad(transform.GetTransform(), sprite.Texture, sprite.TilingFactor, sprite.Color);
                else if (sprite.SubTexComp)
                    Renderer2D::DrawQuad(transform.GetTransform(), sprite.SubTexComp, sprite.TilingFactor, sprite.Color);
                else
                    Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
            }
            Renderer2D::EndScene();
        }
        else
        {
            // TODO: Should it be assert or Warning
//            IK_CORE_WARN("No Camera is Binded to the Scene or any one of themis not set to primary !!! ");
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
    
    Entity Scene::GetLightEntity()
    {
        auto view = m_Registry.view<LightComponent>();
        for (auto entity : view)
        {
            auto& comp = view.get<LightComponent>(entity);
            if (comp.IsLight)
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

    template<>
    void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
    {
    }
    
    template<>
    void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {
    }
    
    template<>
    void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {
        component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
    }
    
    template<>
    void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
    {
    }
    
    template<>
    void Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component)
    {
    }
    
    template<>
    void Scene::OnComponentAdded<LightComponent>(Entity entity, LightComponent& component)
    {
    }

}
