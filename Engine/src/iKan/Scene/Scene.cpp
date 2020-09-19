#include <iKan/Scene/Scene.h>
#include <iKan/Scene/Components.h>
#include <iKan/Scene/Entity.h>

#include <iKan/Renderer/Renderer2D.h>

namespace iKan {
    
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
        
        // Render 2D Sprites
        Camera* mainCamera         = nullptr;
        glm::mat4* cameraTransform = nullptr;
        
        // Get the camera component and its transform matrix to Beine the Scene
        auto view = m_Registry.view<TransformComponent, CameraComponent>();
        for (auto entity : view)
        {
            // As we can have multiple cameras so pic the first camera that is set to primary and rendere acc to its properties
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
        
    Collisions Scene::CollisionDetection(Entity& ce, Speeds speeds)
    {
        /* ce: Current Entity is cosidered as the bock which is moving and in respect of which collision needs to be detected*/
        
        // to store the Results of Collision types in Bits
        Collisions result = CollisionBit::NoCollision;
        
        // pixel of current Entity that should be colloided with the Entity to consider valid Collision
        constexpr float pixelCollision = 0.0f;

        // Extracting Tag and Position of Current Enitity
        std::string ceTag = ce.GetComponent<TagComponent>().Tag;
        glm::vec2   cePos = { ce.GetComponent<TransformComponent>().Transform[3][0], ce.GetComponent<TransformComponent>().Transform[3][1] };
//        IK_CORE_INFO("{0}, {1}", cePos.x, cePos.y);
    
        // Extracting the cell size of Current Entity
        auto ceSprite          = ce.GetComponent<SpriteRendererComponent>();
        glm::vec2 ceCellSize   = { 16.0f, 16.0f };
        glm::vec2 ceSpriteSize = { 16.0f, 16.0f };

        if (auto subTexComp = ceSprite.SubTexComp)
        {
            ceCellSize   = subTexComp->GetCellSize();
            ceSpriteSize = subTexComp->GetSpriteSize();
        }
        else
        {
            // TODO: Add collision size handling for Color entity too
        }
        
        /*
         fraction of block(in decimal) that should be considered as valid collision : ex. 0.1875 part of block should be colloided for now
         this fraction is depending on the current entity Cell size which is 16.0 x 16.0 by default
         e.g { (16 - 3) / 16 , (16 - 3) / 16 }
         */
        glm::vec2 fractionCollision = { (ceCellSize.x - pixelCollision) / ceCellSize.x, (ceCellSize.y - pixelCollision) / ceCellSize.y };

        /*
         It is the offset between the two entites that are about to get colloided
         if Current Entity Size is not equal to 16 x 16 (1 x 1) then we need to add some offset for collision depending on the size of Entity
         Here Adding the offset of Current Entity only later will add the Collision entity offset too in this
         e.g. if sprite size is 1 x 2 : { 1 + (1 - 1) / 2, 1 + (2 - 2) / 2 } = { 1, 1.5 }
         */
        glm::vec2 ceCollisionOffset = { 1.0f + ((ceSpriteSize.x - 1.0f) / 2), 1.0f + ((ceSpriteSize.y - 1.0f)) / 2 };

        auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
        for (auto& entity : view)
        {
            auto [transform, tag, sprite] = m_Registry.get<TransformComponent, TagComponent, SpriteRendererComponent>(entity);
            if (ceTag != tag.Tag)
            {
                glm::vec2 entitySpriteSize = sprite.SubTexComp->GetSpriteSize();
                glm::vec2 entityPos        = { transform.Transform[3][0], transform.Transform[3][1] };

                // NOTE: we are not adding 1.0f in this like we done in ceCollisionOffset because it is for extra offset only and 1.0f (next offset) is considered already
                glm::vec2 collisionOffset  = { ceCollisionOffset.x + ((entitySpriteSize.x - 1.0f) / 2), ceCollisionOffset.y + ((entitySpriteSize.y - 1.0f) / 2) };

                /* Up Down Collision */
                // if Entity sprite size is 2 (taking area of 2 block and cosider as one entity) then add 0.5 on both side of collision fraction
                /*
                 -------------------------------------------------------------------------------------------------------------------------------------------------
                             |---| y = 1
                             |___|
                               |---|                ce (x, y) x = 0.4, y = 0.0f, speed 0.1
                               |___|             consider entities for collision
                        |---||---||---|         (0.4 + 0.8125( 1.2125 ) >= -1.0) && (0.4 - 0.8125( -0.4125 )  <= -1.0)  APPLICABLE
                        |___||___||___|         (0.4 + 0.8125( 1.2125 ) >=  0.0) && (0.4 - 0.8125( -0.4125 )  <=  0.0)  APPLICABLE
                    x =  -1    0    1           (0.4 + 0.8125( 1.2125 ) >=  1.0) && (0.4 - 0.8125( -0.4125 ) !<=  1.0)  NOT APPLICABLE

                    y =  -1   -1   -1           (0.0 - 0.1 ( -0.1 ) <= -1.0f + 1.0 ( 0.0 )) && (0.0 - 0.1 ( -0.1 )  >= -1.0) APPLICABLE
                    y =   1    1    1           (0.0 - 0.1 ( -0.1 ) <=  1.0f + 1.0 ( 2.0 )) && (0.0 - 0.1 ( -0.1 ) !>=  1.0) NOT APPLICABLE
                    y =   0    0    0           (0.0 - 0.1 ( -0.1 ) <=  0.0f + 1.0 ( 1.0 )) && (0.0 - 0.1 ( -0.1 ) !>=  0.0) NOT APPLICABLE
                 -------------------------------------------------------------------------------------------------------------------------------------------------
                                |---|               ce (x, y) x = 0.9, y = 0.0f, speed 0.1
                                |___|           consider entities for collision
                        |---||---||---|         (0.9 + 0.8125( 1.7125 ) >= -1.0) && (0.9 - 0.8125( 0.0875 ) !<= -1.0)  NOT APPLICABLE
                        |___||___||___|         (0.9 + 0.8125( 1.7125 ) >=  0.0) && (0.9 - 0.8125( 0.0875 ) !<=  0.0)  NOT APPLICABLE
                    x =  -1    0    1           (0.9 + 0.8125( 1.7125 ) >=  1.0) && (0.9 - 0.8125( 0.0875 )  <=  1.0)  APPLICABLE

                    y =  -1   -1   -1           (0.0 - 0.1 ( -0.1 ) <= -1.0f + 1.0 ( 0.0 )) && (0.0 - 0.1 ( -0.1 )  >= -1.0) APPLICABLE
                    y =   1    1    1           (0.0 - 0.1 ( -0.1 ) <=  1.0f + 1.0 ( 2.0 )) && (0.0 - 0.1 ( -0.1 ) !>=  1.0) NOT APPLICABLE
                    y =   0    0    0           (0.0 - 0.1 ( -0.1 ) <=  0.0f + 1.0 ( 1.0 )) && (0.0 - 0.1 ( -0.1 ) !>=  0.0) NOT APPLICABLE
                 -------------------------------------------------------------------------------------------------------------------------------------------------
                          |---|                     ce (x, y) x = -0.9, y = 0.0f, speed 0.1
                          |___|                 consider entities for collision
                        |---||---||---|         (-0.9 + 0.8125( -0.0875 )  >= -1.0) && (-0.9 - 0.8125( -1.7125 ) <= -1.0)  APPLICABLE
                        |___||___||___|         (-0.9 + 0.8125( -0.0875 ) !>=  0.0) && (-0.9 - 0.8125( -1.7125 ) <=  0.0)  NOT APPLICABLE
                    x =  -1    0    1           (-0.9 + 0.8125( -0.0875 ) !>=  1.0) && (-0.9 - 0.8125( -1.7125 ) <=  1.0)  NOT APPLICABLE

                    y =  -1   -1   -1           (0.0 - 0.1 ( -0.1 ) <= -1.0f + 1.0 ( 0.0 )) && (0.0 - 0.1 ( -0.1 )  >= -1.0) APPLICABLE
                    y =   1    1    1           (0.0 - 0.1 ( -0.1 ) <=  1.0f + 1.0 ( 2.0 )) && (0.0 - 0.1 ( -0.1 ) !>=  1.0) NOT APPLICABLE
                    y =   0    0    0           (0.0 - 0.1 ( -0.1 ) <=  0.0f + 1.0 ( 1.0 )) && (0.0 - 0.1 ( -0.1 ) !>=  0.0) NOT APPLICABLE
                 -------------------------------------------------------------------------------------------------------------------------------------------------
                 */
                if (float xCollDiff = fractionCollision.x + ((entitySpriteSize.x - 1) / 2.0f);
                    (cePos.x + xCollDiff) > entityPos.x && (cePos.x - xCollDiff) < entityPos.x)
                {
                    // if the next Y position of current entity is coilloiding with the entity which is just bellow the curr entity
                    if((cePos.y - speeds.Down <= entityPos.y + collisionOffset.y) && (cePos.y - speeds.Down > entityPos.y))
                    {
                        result |= CollisionBit::Down;
                    }
                }

                /* Right Left Collision */
                /*
                 -------------------------------------------------------------------------------------------------------------------------------------------------
                        |---|                       ce (x, y) x = 0, y = -7.3f, speed 0.1
                        |___||---|              consider entities for collision
                             |___|              (-7.3  >= -8.5) && (-0.9 - 0.8125( -1.7125 )  <= -1.0)  APPLICABLE
                 -------------------------------------------------------------------------------------------------------------------------------------------------
                 */
                if (((cePos.y /* + fractionCollision.y */) >= entityPos.y && (cePos.y /*- fractionCollision.y*/) <= entityPos.y + 1) ||
                    ((cePos.y /* + fractionCollision.y */) <= entityPos.y && (cePos.y /*- fractionCollision.y*/) >= entityPos.y - 1))
                {
                    /* Right Collision */
                    // if the next X position of current entity is coilloiding with the entity which is just Right of the curr entity
                    // adding 0.5 to 1 as if collision entity size is 2 then 0.5 shold be added to its core position (center)
                    if ((cePos.x + speeds.Right <= entityPos.x) && (cePos.x + speeds.Right >= entityPos.x - collisionOffset.x))
                    {
                        result |= CollisionBit::Right;
                    }

                    /* Left Collision */
                    // if the next X position of current entity is coilloiding with the entity which is just Left of the curr entity
                    if ((cePos.x - speeds.Right <= entityPos.x + collisionOffset.x) && (cePos.x - speeds.Right > entityPos.x))
                    {
                        result |= CollisionBit::Left;
                    }

                }

            }
        }
        return result;
    }
    
}
