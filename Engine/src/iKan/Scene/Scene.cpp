#include <iKan/Scene/Scene.h>
#include <iKan/Scene/Components.h>
#include <iKan/Scene/Entity.h>

#include <iKan/Renderer/Renderer2D.h>
#include <iKan/Renderer/Renderer3D.h>
#include <iKan/Renderer/SceneRenderer.h>

namespace iKan {
    
    Scene::Scene(SceneRendererType sceneRenderer)
    : m_ScceneRenderer(sceneRenderer)
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
    
    // Designed for 3D only
    void Scene::OnEditorUpdate(TimeStep ts, const EditorCamera& camera)
    {
        SceneRenderer::BeginScene(this, { camera, camera.GetViewMatrix() });
        SceneRenderer::Draw();
    }
    
    void Scene::OnUpdate(TimeStep ts)
    {
        // For all Entity having Native Scripts just instantiate the Scrips Binded to them and update them
        InstantiateScripts(ts);
        
        if (Entity cameraEntity = GetMainCameraEntity();
            cameraEntity!= Entity(entt::null, this))
        {
            m_MainCamera      = &cameraEntity.GetComponent<CameraComponent>().Camera;
            m_CameraTransform = &cameraEntity.GetComponent<TransformComponent>().Transform;
        }
        
        // Renderer
        if (m_MainCamera)
        {
            switch (m_ScceneRenderer)
            {
                case SceneRendererType::_2D: Renderer2D(); break;
                case SceneRendererType::_3D: Renderer3D(); break;
                default: IK_CORE_ASSERT(false, "Invalid Scene Renderer");
            }
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
    
    void Scene::Renderer2D()
    {
        Renderer2D::BeginScene(m_MainCamera->GetProjection(), *m_CameraTransform);
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
    
    void Scene::Renderer3D()
    {
        Renderer3D::BeginScene(m_MainCamera->GetProjection(), *m_CameraTransform);
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            const auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer3D::DrawCube(transform, sprite.Color);
        }
        Renderer3D::EndScene();
    }
    
    // TODO: For now only for Mario Branch Need to be fix later
    Collisions Scene::CollisionDetection(Entity& ce, Speeds speeds)
    {
        /* ce: Current Entity is cosidered as the bock which is moving and in respect of which collision needs to be detected*/
        
        // to store the Results of Collision types in Bits
        Collisions result = CollisionBit::NoCollision;
        
        // pixel of current Entity that should be colloided with the Entity to consider valid Collision
        constexpr float pixelCollision = 3.0f;

        // Extracting Tag and Position of Current Enitity
        std::string ceTag = ce.GetComponent<TagComponent>().Tag;
        glm::vec2   cePos = { ce.GetComponent<TransformComponent>().Transform[3][0], ce.GetComponent<TransformComponent>().Transform[3][1] };
    
        // Extracting the cell size of Current Entity
        auto ceSprite        = ce.GetComponent<SpriteRendererComponent>();
        glm::vec2 ceCellSize = { 16.0f, 16.0f };
        
        if (auto subTexComp = ceSprite.SubTexComp)
            ceCellSize = subTexComp->GetCellSize();
        
        // fraction of block(in decimal) that should be considered as valid collision : ex. 0.1875 part of block should be colloided for now
        // this fraction is depending on the current entity Cell size which is 16.0 x 16.0 by default
        glm::vec2 fractionCollision = { (ceCellSize.x - pixelCollision) / ceCellSize.x, (ceCellSize.y - pixelCollision) / ceCellSize.y };
        
        auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
        for (auto& entity : view)
        {
            auto [transform, tag, sprite] = m_Registry.get<TransformComponent, TagComponent, SpriteRendererComponent>(entity);
            glm::vec2 entitySpriteSize    = sprite.SubTexComp->GetSpriteSize();
            glm::vec2 entityPos           = { transform.Transform[3][0], transform.Transform[3][1] };
            
            if (ceTag != tag.Tag)
            {
                /* Up Down Collision*/
                // if Entity sprite size is 2 (taking are of 2 block and cosider as one entity) then add 0.5 on both side of collision fraction
                // TODO: for other size i.e. 3, 4, ... changes are required to be done
                if (float xCollDiff = fractionCollision.x + ((entitySpriteSize.x == 2.0f) ? 0.5f : 0.0f);
                    (cePos.x + xCollDiff) >= entityPos.x && (cePos.x - xCollDiff) <= entityPos.x)
                {
                    // if the next Y position of current entity is coilloiding with the entity which is just bellow the curr entity
                    if((cePos.y - speeds.Down <= entityPos.y + 1) && (cePos.y - speeds.Down > entityPos.y))
                        result |= CollisionBit::Down;
                }
                
                /* Right Left Collision */
                if (float yCollDiff = fractionCollision.y;
                    (cePos.y + yCollDiff) >= entityPos.y && (cePos.y - yCollDiff) <= entityPos.y)
                {
                    /* Right Collision */
                    // if the next X position of current entity is coilloiding with the entity which is just Right of the curr entity
                    if (float nextEntityDiff = (entitySpriteSize.x == 1.0f) ? 1.0f : 1.5f;
                        (cePos.x + speeds.Right <= entityPos.x) && (cePos.x + speeds.Right > entityPos.x - nextEntityDiff))
                    {
                        result |= CollisionBit::Right;
                    }
                    
                    /* Left Collision */
                    // if the next X position of current entity is coilloiding with the entity which is just Left of the curr entity
                    if (float nextEntityDiff = (entitySpriteSize.x == 1.0f) ? 1.0f : 1.5f;
                        (cePos.x - speeds.Right <= entityPos.x + nextEntityDiff) && (cePos.x - speeds.Right > entityPos.x))
                    {
                        result |= CollisionBit::Left;
                    }

                }

            }
        }
        return result;
    }
    
}
