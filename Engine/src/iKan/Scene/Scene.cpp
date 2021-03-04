// ******************************************************************************
//   File    : Scene.cpp
//   Project : i-Kan : Scene
//
//   Created by Ashish
// ******************************************************************************

#include <iKan/Scene/Scene.h>
#include <iKan/Scene/Components.h>
#include <iKan/Scene/Entity.h>

#include <iKan/Renderer/API/Renderer2D.h>
#include <iKan/Renderer/API/SceneRenderer.h>

#include <glad/glad.h>

namespace iKan {

    // ******************************************************************************
    // Get the set bit postition (if value is power of 2)
    // ******************************************************************************
    static int32_t GetBitPos(int32_t value)
    {
        if (!value)
            return -1;
        return log2(value);
    }

    // ******************************************************************************
    // Scene Constructor
    // ******************************************************************************
    Scene::Scene()
    {
        IK_CORE_INFO("Creating Scene !!");
    }
    
    // ******************************************************************************
    // Scene Destructor
    // ******************************************************************************
    Scene::~Scene()
    {
        IK_CORE_WARN("Destroying Scene !!");
    }
    
    // ******************************************************************************
    // Create Entity in Scene
    // ******************************************************************************
    Entity Scene::CreateEntity(const std::string& name)
    {
        IK_CORE_TRACE("Creating Entity in Scene, Name: {0}", name.c_str());

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
    
    // ******************************************************************************
    // Create Entity in Scene with UUID
    // ******************************************************************************
    Entity Scene::CreateEntityWithID(UUID uuid, const std::string& name)
    {
        IK_CORE_TRACE("Creating Entity in Scene, Name: {0}, ID: {1}", name.c_str(), uuid);

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

    // ******************************************************************************
    // Destroy Entity from Scene
    // ******************************************************************************
    void Scene::DestroyEntity(Entity entity)
    {
        IK_CORE_WARN("Destrying Entity '{0}' from the scene", entity.GetComponent<TagComponent>().Tag.c_str());
        m_Registry.destroy(entity);
    }
    
    // ******************************************************************************
    // Update Sene Editor
    // ******************************************************************************
    void Scene::OnUpdateEditor(TimeStep ts, EditorCamera &editorCamera)
    {
        // Get the Light
        {
            auto lightView = m_Registry.view<LightComponent>();
            for (auto entity : lightView)
            {
                auto& lc = lightView.get<LightComponent>(entity);
                Entity lightEnt = { entity, this };
                SceneLight light = lc.Light;
                glm::vec3  lightPosition = lightEnt.GetComponent<TransformComponent>().Translation;

                SceneRenderer::SetupLight({ lc.IsLight, light, lightPosition, editorCamera.GetPosition(), editorCamera.GetForwardDirection() });
            }
        }

        // Scene Mesh
        {
            SceneRenderer::BegineScene({ editorCamera, editorCamera.GetViewMatrix() });
            auto meshGroup = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
            for (auto entity : meshGroup)
            {
                const auto [transform, mesh] = meshGroup.get<TransformComponent, MeshComponent>(entity);
                if (mesh.Mesh)
                    SceneRenderer::Draw(mesh.Mesh, transform.GetTransform());
            }
            SceneRenderer::EndScene();
        }

        // 2D Renderer
        {
            Renderer2D::BeginScene(editorCamera);
            auto spriteGroup = m_Registry.group<>(entt::get<TransformComponent, SpriteRendererComponent>);
            for (auto entity : spriteGroup)
            {
                const auto [transform, sprite] = spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);
                if (sprite.Texture)
                    Renderer2D::DrawQuad(transform.GetTransform(), sprite.Texture, sprite.TilingFactor, sprite.Color);
                else if (sprite.SubTexComp)
                    Renderer2D::DrawQuad(transform.GetTransform(), sprite.SubTexComp, sprite.TilingFactor, sprite.Color);
                else
                    Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (int32_t)entity);
            }
            Renderer2D::EndScene();
        }
    }

    // ******************************************************************************
    // Get the pixel from scene
    // ******************************************************************************
    int32_t Scene::Pixel(int32_t mx, int32_t my)
    {
        glReadBuffer(GL_COLOR_ATTACHMENT1);
        int32_t pixelData = -1;
        glReadPixels(mx, my, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

        return pixelData;
    }

    // ******************************************************************************
    // Update runtime Scene
    // ******************************************************************************
    void Scene::OnUpdateRuntime(TimeStep ts)
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
                    Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (int32_t)entity);
            }
            Renderer2D::EndScene();
        }
        else
        {
            // TODO: Should it be assert or Warning
//            IK_CORE_WARN("No Camera is Binded to the Scene or any one of themis not set to primary !!! ");
        }
    }
    
    // ******************************************************************************
    // Resize scene view port
    // ******************************************************************************
    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        IK_CORE_INFO("Scene Viewport resized to {0} x {1}", width, height);
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
     
    // ******************************************************************************
    // get the main canera component
    // ******************************************************************************
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
    
    // ******************************************************************************
    // get the main Light component
    // ******************************************************************************
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
    
    // ******************************************************************************
    // Instantiate the native script component of each entity
    // ******************************************************************************
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
    
    // ******************************************************************************
    // Chech the box Collision : Only for 2D
    // ******************************************************************************
    int32_t Scene:: BoxCollisionDetection(Entity& currEntity, float speed)
    {
        // ******************************************************************************
        // To store the entity of collisions. storing 4 as an entityt can be colloided
        // with 4 entitiy at one time (only Box collision)
        // ******************************************************************************
        static Entity s_BoxColloidedEntity[4];

        int32_t result = 0;
        const auto& currEntPos  = currEntity.GetComponent<TransformComponent>().Translation;
        const auto& currEntSize = currEntity.GetComponent<TransformComponent>().Scale;

        auto group = m_Registry.group<>(entt::get<TransformComponent, BoxCollider2DComponent>);
        for (auto entity : group)
        {
            if (currEntity == entity)
                continue;

            const auto [transform, boxColl] = group.get<TransformComponent, BoxCollider2DComponent>(entity);
            if (boxColl.IsRigid)
            {
                const auto& entPos  = transform.Translation;
                const auto& entSize = transform.Scale;
                
                // Illustration for Right Left Face overlap
                {
                    /*
                     Case 1.            Case 2.             Case 3.
                                            |--|
                     |--||--|           |--||__|            |--|
                     |__||__|           |__|                |__||--|
                                                                |__|
                     */
                }
                if ((currEntPos.y + (std::abs(currEntSize.y) / 2) > entPos.y - (std::abs(entSize.y) / 2)) &&  // Case 2
                    (currEntPos.y - (std::abs(currEntSize.y) / 2) < entPos.y + (std::abs(entSize.y) / 2)))    // Case 3
                {
                    // Right collision of current entity
                    if ((currEntPos.x + speed + (std::abs(currEntSize.x) / 2) >= entPos.x - (std::abs(entSize.x) / 2)) &&
                        (currEntPos.x - (std::abs(currEntSize.x) / 2) < entPos.x + (std::abs(entSize.x) / 2)))
                    {
                        result |= (int32_t)BoxCollisionSide::Right;
                        s_BoxColloidedEntity[GetBitPos((int32_t)BoxCollisionSide::Right)] = Entity(entity, this);
                    }
                    // Left collision of current entity
                    if ((currEntPos.x + (std::abs(currEntSize.x) / 2) > entPos.x - (std::abs(entSize.x) / 2)) &&
                        (currEntPos.x - speed - (std::abs(currEntSize.x) / 2) <= entPos.x + (std::abs(entSize.x) / 2)))
                    {
                        result |= (int32_t)BoxCollisionSide::Left;
                        s_BoxColloidedEntity[GetBitPos((int32_t)BoxCollisionSide::Left)] = Entity(entity, this);
                    }
                }
                
                // Illustration for Up Down Face overlap
                {
                    /*
                     Case 1.            Case 2.             Case 3.
                     |--|               |--|                  |--|
                     |__|               |__|                  |__|
                     |--|                 |--|              |--|
                     |__|                 |__|              |__|

                     */
                }
                if ((currEntPos.x + (std::abs(currEntSize.x) / 2) > entPos.x - (std::abs(entSize.x) / 2)) &&  // Case 2
                    (currEntPos.x - (std::abs(currEntSize.x) / 2) < entPos.x + (std::abs(entSize.x) / 2)))    // Case 3
                {
                    // Up collision of current entity
                    if ((currEntPos.y + speed + (std::abs(currEntSize.y) / 2) >= entPos.y - (std::abs(entSize.y) / 2)) &&
                        (currEntPos.y - (std::abs(currEntSize.y) / 2) < entPos.y - (std::abs(entSize.y) / 2)))
                    {
                        result |= (int32_t)BoxCollisionSide::Up;
                        s_BoxColloidedEntity[GetBitPos((int32_t)BoxCollisionSide::Up)] = Entity(entity, this);
                    }
                    // Down collision of current entity
                    if ((currEntPos.y + (std::abs(currEntSize.y) / 2) > entPos.y - (std::abs(entSize.y) / 2)) &&
                        (currEntPos.y - speed - (std::abs(currEntSize.y) / 2) <= entPos.y + (std::abs(entSize.y) / 2)))
                    {
                        result |= (int32_t)BoxCollisionSide::Down;
                        s_BoxColloidedEntity[GetBitPos((int32_t)BoxCollisionSide::Down)] = Entity(entity, this);
                    }
                }
            } // if (boxColl.IsRigid)
        } // for (auto entity : group)

        if (result)
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
                    script->OnBoxCollision(s_BoxColloidedEntity, result);
                }
            });
        }
        return result;
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
    
    template<>
    void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
    {
    }

}
