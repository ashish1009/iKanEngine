#include <iKan/Editor/SceneHierarchyPannel.h>
#include <iKan/Core/Math.h>

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
        
        ImGui::End();
        
        ImGui::Begin("Properties");
        if (m_SelectionContext)
            DrawComponents(m_SelectionContext);
        
        ImGui::End();
    }

    void SceneHeirarchyPannel::DrawEntityNode(Entity entity)
    {
#pragma clang diagnostic ignored "-Wformat-security"
        auto& tag = entity.GetComponent<TagComponent>().Tag;
        
        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }
        
        if (opened)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
            bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
            if (opened)
                ImGui::TreePop();
            ImGui::TreePop();
        }
    }
    
    void SceneHeirarchyPannel::DrawComponents(Entity entity)
    {
        if (entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>().Tag;
            
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, tag.c_str());
            
            ImGui::Columns(2);
            ImGui::Text("Tag");
            ImGui::NextColumn();
            ImGui::PushItemWidth(-1);

            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }
            
            ImGui::PopItemWidth();
            ImGui::NextColumn();
            ImGui::Columns(1);
            
            ImGui::Separator();
        }
        
        if (entity.HasComponent<TransformComponent>())
        {
            auto& tc = entity.GetComponent<TransformComponent>();
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
            {
                auto [translation, rotationQuat, scale] = Math::GetTransformDecomposition(tc);
                glm::vec3 rotation = glm::degrees(glm::eulerAngles(rotationQuat));

                ImGui::Columns(2);
                ImGui::Text("Translation");
                ImGui::NextColumn();
                ImGui::PushItemWidth(-1);
                
                bool updateTransform = false;
                
                if (ImGui::DragFloat3("##translation", glm::value_ptr(translation), 0.25f))
                {
                    updateTransform = true;
                }
                
                ImGui::PopItemWidth();
                ImGui::NextColumn();
                
                ImGui::Text("Rotation");
                ImGui::NextColumn();
                ImGui::PushItemWidth(-1);
                
                if (ImGui::DragFloat3("##rotation", glm::value_ptr(rotation), 0.25f))
                {
                    updateTransform = true;
                }
                
                ImGui::PopItemWidth();
                ImGui::NextColumn();
                
                ImGui::Text("Scale");
                ImGui::NextColumn();
                ImGui::PushItemWidth(-1);
                
                if (ImGui::DragFloat3("##scale", glm::value_ptr(scale), 0.25f))
                {
                    updateTransform = true;
                }
                
                ImGui::PopItemWidth();
                ImGui::NextColumn();
                
                ImGui::Columns(1);
                
                if (updateTransform)
                {
                    tc.Transform = glm::translate(glm::mat4(1.0f), translation) *
                    glm::toMat4(glm::quat(glm::radians(rotation))) *
                    glm::scale(glm::mat4(1.0f), scale);
                }
                
                ImGui::TreePop();
            }
            ImGui::Separator();
        }
        
        if (entity.HasComponent<CameraComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
            {
                auto& cameraComponent = entity.GetComponent<CameraComponent>();
                auto& camera = cameraComponent.Camera;
                
                ImGui::Columns(2);
                
                ImGui::Text("Primary");
                ImGui::NextColumn();
                ImGui::PushItemWidth(-1);
                
                ImGui::Checkbox("##Primary", &cameraComponent.Primary);
                
                ImGui::PopItemWidth();
                ImGui::NextColumn();
                
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
                
                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
                {
                    ImGui::Text("Size");
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(-1);
                    
                    float orthoSize = glm::degrees(camera.GetOrthographicSize());
                    if (ImGui::DragFloat("##Size", &orthoSize))
                        camera.SetOrthographicSize(glm::radians(orthoSize));
                    
                    ImGui::PopItemWidth();
                    ImGui::NextColumn();
                    
                    ImGui::Text("Near");
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(-1);
                    
                    float nearClip = camera.GetOrthographicNearClip();
                    if (ImGui::DragFloat("##Near", &nearClip))
                        camera.SetOrthographicNearClip(nearClip);
                    
                    ImGui::PopItemWidth();
                    ImGui::NextColumn();
                    
                    ImGui::Text("Far");
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(-1);
                    
                    float farClip = camera.GetOrthographicFarClip();
                    if (ImGui::DragFloat("##Far", &farClip))
                        camera.SetOrthographicFarClip(farClip);
                    
                    ImGui::PopItemWidth();
                    ImGui::NextColumn();
                                        
                    ImGui::Text("Fixed Aspect Ratio"); ImGui::NextColumn(); ImGui::PushItemWidth(-1);
                    ImGui::Checkbox("##Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
                    
                    ImGui::PopItemWidth();
                    ImGui::NextColumn();
                }

                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
                {
                    ImGui::Text("FOV");
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(-1);
                    
                    float verticalFov = glm::degrees(camera.GetPerspectiveFOV());
                    if (ImGui::DragFloat("##verticalFOV", &verticalFov))
                        camera.SetPerspectiveFOV(glm::radians(verticalFov));
                    
                    ImGui::PopItemWidth();
                    ImGui::NextColumn();

                    ImGui::Text("Near");
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(-1);
                    
                    float nearClip = camera.GetPerspectiveNearClip();
                    if (ImGui::DragFloat("##Near", &nearClip))
                        camera.SetPerspectiveNearClip(nearClip);
                    
                    ImGui::PopItemWidth();
                    ImGui::NextColumn();

                    ImGui::Text("Far");
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(-1);
                    
                    float farClip = camera.GetPerspectiveFarClip();
                    if (ImGui::DragFloat("##Far", &farClip))
                        camera.SetPerspectiveFarClip(farClip);
                    
                    ImGui::PopItemWidth();
                    ImGui::NextColumn();
                }

                ImGui::TreePop();
            }
            ImGui::Separator();
        }
    }
    
}
