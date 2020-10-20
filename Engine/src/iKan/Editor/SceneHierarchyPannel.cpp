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
            
            BeginPropertyGrid();
            Property("Tag", tag);
            EndPropertyGrid();

            ImGui::Separator();
        } // if (entity.HasComponent<TagComponent>())
        
        if (entity.HasComponent<TransformComponent>())
        {
            auto& tc = entity.GetComponent<TransformComponent>();
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
            {
                auto [translation, rotationQuat, scale] = GlmMath::GetTransformDecomposition(tc);
                glm::vec3 rotation = glm::degrees(glm::eulerAngles(rotationQuat));
                bool updateTransform = false;

                BeginPropertyGrid();
                updateTransform = Property("Translation", translation, 0.25f) | Property("Rotation", rotation, 0.25f) | Property("Scale", scale, 0.25f);
                EndPropertyGrid();
                
                if (updateTransform)
                {
                    tc.Transform = glm::translate(glm::mat4(1.0f), translation) *
                    glm::toMat4(glm::quat(glm::radians(rotation))) *
                    glm::scale(glm::mat4(1.0f), scale);
                }
                
                ImGui::TreePop();
            }
            ImGui::Separator();
        } // if (entity.HasComponent<TransformComponent>())
        
        if (entity.HasComponent<CameraComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
            {
                auto& cameraComponent = entity.GetComponent<CameraComponent>();
                auto& camera = cameraComponent.Camera;
                
                ImGui::Columns(2);
                
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
    }
    
}


