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
            DrawComponents(m_SelectionContext);
        
        ImGui::End();
    }

    void SceneHeirarchyPannel::DrawEntityNode(Entity entity)
    {
        auto& tag = entity.GetComponent<TagComponent>().Tag;
                
        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

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
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
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
    
    template<typename T, typename UIFunction>
    static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
    {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen |
                                                 ImGuiTreeNodeFlags_Framed |
                                                 ImGuiTreeNodeFlags_SpanAvailWidth |
                                                 ImGuiTreeNodeFlags_AllowItemOverlap |
                                                 ImGuiTreeNodeFlags_FramePadding;
        if (entity.HasComponent<T>())
        {
            auto& component = entity.GetComponent<T>();
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
            
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
            ImGui::PopStyleVar();
            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
            {
                ImGui::OpenPopup("ComponentSettings");
            }
            
            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                if (ImGui::MenuItem("Remove component"))
                    removeComponent = true;
                
                ImGui::EndPopup();
            }
            
            if (open)
            {
                uiFunction(component);
                ImGui::TreePop();
            }
            
            if (removeComponent)
                entity.RemoveComponent<T>();
        }
    }
    
    void SceneHeirarchyPannel::DrawComponents(Entity entity)
    {
        if (entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>().Tag;
            Property("Tag", tag);
            ImGui::Separator();
        }
        
        ImGui::SameLine();
        ImGui::PushItemWidth(-1);
        
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
        
        ImGui::PopItemWidth();
        
        DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
                                          {
            Property("Translation", component.Translation, 0.25f);
            glm::vec3 rotation = glm::degrees(component.Rotation);
            Property("Rotation", rotation, 0.25f);
            component.Rotation = glm::radians(rotation);
            Property("Scale", component.Scale, 0.25f, 1.0f);
        });
        
        DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
                                       {
            auto& camera = component.Camera;
            
            Property("Primary", component.Primary);
            ImGui::Separator();

            {
                ImGui::Columns(2);
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
                ImGui::Columns(1);
            }
            ImGui::Separator();

            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
            {
                float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveFOV());
                if (Property("Vertical FOV", perspectiveVerticalFov))
                    camera.SetPerspectiveFOV(glm::radians(perspectiveVerticalFov));
                
                float perspectiveNear = camera.GetPerspectiveNearClip();
                if (Property("Near", perspectiveNear))
                    camera.SetPerspectiveNearClip(perspectiveNear);
                
                float perspectiveFar = camera.GetPerspectiveFarClip();
                if (Property("Far", perspectiveFar))
                    camera.SetPerspectiveFarClip(perspectiveFar);
            }
            
            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
            {
                float orthoSize = camera.GetOrthographicSize();
                if (Property("Size", orthoSize))
                    camera.SetOrthographicSize(orthoSize);
                
                float orthoNear = camera.GetOrthographicNearClip();
                if (Property("Near", orthoNear))
                    camera.SetOrthographicNearClip(orthoNear);
                
                float orthoFar = camera.GetOrthographicFarClip();
                if (Property("Far", orthoFar))
                    camera.SetOrthographicFarClip(orthoFar);
                
                ImGui::Separator();
                Property("Fixed Aspect Ratio", component.FixedAspectRatio);
            }
        });
        
        DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
                                               {
            ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
        });
        
        DrawComponent<LightComponent>("Light", entity, [](auto& component)
                                               {
            ImGui::Checkbox("Is Scene Light", &component.IsLight);

            if (component.IsLight)
            {
                auto& light = component.Light;

                ImGui::Checkbox("Is Ambient", &light.IsAmbient);
                if (light.IsAmbient)
                    Property("Ambient", light.Ambient, 0.1f);

                ImGui::Checkbox("Is Diffuse", &light.IsDiffuse);
                if (light.IsDiffuse)
                    Property("Diffuse", light.Diffuse, 0.1f);
                
                ImGui::Checkbox("Is Specular", &light.IsSpecular);
                if (light.IsSpecular)
                    Property("Specular", light.Specular, 0.1f);
            }
        });
        
    }

}


