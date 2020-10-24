#include <iKan/Editor/SceneHierarchyPannel.h>
#include <iKan/Editor/SceneProperyGrid.h>
#include <iKan/Core/GlmMath.h>

namespace iKan {
    
    SceneHeirarchyPannel::SceneHeirarchyPannel(const Ref<Scene>& context)
    {
        SetContext(context);
    }
    
    void SceneHeirarchyPannel::SetContext(const Ref<Scene>& context)
    {
        m_Context = context;
    }
    
    void SceneHeirarchyPannel::OnImguiender()
    {
        ImGui::Begin("Scene Hierarchy");
        
        m_Context->m_Registry.each([&](auto entityID)
        {
            Entity entity( { entityID, m_Context.Raw() } );
            DrawEntityNode(entity);
        });
        
        if (ImGui::IsMouseDown((int)MouseCode::Button0) && ImGui::IsWindowHovered())
            m_SelectionContext = {};
        
        // false -> Right-click on blank space
        if (ImGui::BeginPopupContextWindow(0, (int)MouseCode::ButtonRight, false))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
                m_Context->CreateEntity("Empty Entity");
            
            ImGui::EndPopup();
        }
        
        ImGui::End();
        
        ImGui::Begin("Properties");
        if (m_SelectionContext)
        {
            DrawComponents(m_SelectionContext);
            
            if (ImGui::Button("Add Component"))
                ImGui::OpenPopup("AddComponent");
            
            if (ImGui::BeginPopup("AddComponent"))
            {
                if (ImGui::MenuItem("Camera"))
                {
                    m_SelectionContext.AddComponent<CameraComponent>();
                    ImGui::CloseCurrentPopup();
                }
                
                if (ImGui::MenuItem("Sprite Renderer"))
                {
                    m_SelectionContext.AddComponent<SpriteRendererComponent>();
                    ImGui::CloseCurrentPopup();
                }
                
                ImGui::EndPopup();
            }
        }
        
        ImGui::End();
    }

    void SceneHeirarchyPannel::DrawEntityNode(Entity entity)
    {
        auto& tag = entity.GetComponent<TagComponent>().Tag;
                
        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }
        
        bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Entity"))
                entityDeleted = true;
            
            ImGui::EndPopup();
        }
        
        if (opened)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
            bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
            if (opened)
                ImGui::TreePop();
            ImGui::TreePop();
        }
        
        if (entityDeleted)
        {
            m_Context->DestroyEntity(entity);
            if (m_SelectionContext == entity)
                m_SelectionContext = {};
        }
    }
    
    void SceneHeirarchyPannel::DrawComponents(Entity entity)
    {
        // TODO: Add a common iterator that do for all the component in the system
        
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

        if (entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>().Tag;
                        
            Property("Tag", tag);
            
            ImGui::Separator();
        } // if (entity.HasComponent<TagComponent>())
        
        if (entity.HasComponent<TransformComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform"))
            {
                auto& tc = entity.GetComponent<TransformComponent>();
                
                Property("Translation", tc.Translation, 0.25f);
                
                glm::vec3 rotation = glm::degrees(tc.Rotation);
                Property("Rotation", rotation, 0.25f);
                tc.Rotation = glm::radians(rotation);
                
                Property("Scale", tc.Scale, 0.25f, 1.0f);
                
                ImGui::TreePop();
            }
            ImGui::Separator();
        } // if (entity.HasComponent<TransformComponent>())
        
        if (entity.HasComponent<CameraComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlags, "Camera"))
            {
                auto& cameraComponent = entity.GetComponent<CameraComponent>();
                auto& camera = cameraComponent.Camera;
                
                Property("Primary", cameraComponent.Primary);
                
                {
                    ImGui::Text("Projection Type");
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(-1);
                    const char* projectionTypeSTring[] = { "Projection", "Orthographic" };
                    const char* currentProjectionType = projectionTypeSTring[(int32_t)camera.GetProjectionType()];
                    if (ImGui::BeginCombo("##Projection", currentProjectionType))
                    {
                        for (int i = 0; i < 2; i++)
                        {
                            bool bIsSelected = currentProjectionType == projectionTypeSTring[i];
                            if (ImGui::Selectable(projectionTypeSTring[i], bIsSelected))
                            {
                                currentProjectionType = projectionTypeSTring[i];
                                camera.SetProjectionType((SceneCamera::ProjectionType)i);
                            }
                            
                            if (bIsSelected)
                            {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        ImGui::EndCombo();
                    }
                    ImGui::PopItemWidth();
                    ImGui::NextColumn();
                }
                
                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
                {
                    float orthoSize = glm::degrees(camera.GetOrthographicSize());
                    if (Property("Size", orthoSize))
                        camera.SetOrthographicSize(glm::radians(orthoSize));
                    
                    float nearClip = camera.GetOrthographicNearClip();
                    if (Property("Near", nearClip))
                        camera.SetOrthographicNearClip(nearClip);
                    
                    float farClip = camera.GetOrthographicFarClip();
                    if (Property("Far", farClip))
                        camera.SetOrthographicFarClip(farClip);
                    
                    Property("Fixed Aspect Ratio", cameraComponent.FixedAspectRatio);
                }

                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
                {
                    float verticalFov = glm::degrees(camera.GetPerspectiveFOV());
                    if (Property("verticalFOV", verticalFov))
                        camera.SetPerspectiveFOV(glm::radians(verticalFov));

                    float nearClip = camera.GetPerspectiveNearClip();
                    if (Property("Near", nearClip))
                        camera.SetPerspectiveNearClip(nearClip);
                    
                    float farClip = camera.GetPerspectiveFarClip();
                    if (Property("Far", farClip))
                        camera.SetPerspectiveFarClip(farClip);
                }

                ImGui::TreePop();
            }
            ImGui::Separator();
        } // if (entity.HasComponent<CameraComponent>())
        
        if (entity.HasComponent<SpriteRendererComponent>())
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Sprite Renderer");
            
            ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
            if (ImGui::Button("...", ImVec2{ 20, 20 }))
            {
                ImGui::OpenPopup("ComponentSettings");
            }
            ImGui::PopStyleVar();
            
            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                if (ImGui::MenuItem("Remove component"))
                    removeComponent = true;
                
                ImGui::EndPopup();
            }
            
            if (open)
            {
                auto& src = entity.GetComponent<SpriteRendererComponent>();
                ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));
                ImGui::TreePop();
            }
            
            if (removeComponent)
                entity.RemoveComponent<SpriteRendererComponent>();
            
        } // if (entity.HasComponent<SpriteRendererComponent>())
    }
    
}


