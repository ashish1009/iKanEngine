#include <iKan/Editor/SceneHierarchyPannel.h>
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
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }
            ImGui::Columns(1);
            
            ImGui::Separator();
        } // if (entity.HasComponent<TagComponent>())
        
        if (entity.HasComponent<MeshComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(MeshComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Mesh"))
            {
                auto& mesh = entity.GetComponent<MeshComponent>();

                auto& meshProp = mesh.Prop;
                
                const char* meshPropString[] = {  "Basic", "ADS", "Light Source", "Reflection", "Refration"};
                const char* currMeshProp     = meshPropString[(int32_t)meshProp];
                if (ImGui::BeginCombo("##Propery", currMeshProp))
                {
                    for (int i = 0; i < 5; i++)
                    {
                        bool bIsSelected = currMeshProp == meshPropString[i];
                        if (ImGui::Selectable(meshPropString[i], bIsSelected))
                        {
                            currMeshProp = meshPropString[i];
                            meshProp     = (MeshComponent::Property)i;
                        }
                        
                        if (bIsSelected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }
                
                if (meshProp == MeshComponent::Property::Refraction)
                {
                    auto& meshMaterial = mesh.Material;
                    const char* mehMaterialString[] = { "Air", "Water", "Ice", "Glass", "Diamond" };
                    const char* currMaterial        = mehMaterialString[(int32_t)meshMaterial];
                    if (ImGui::BeginCombo("##Material", currMaterial))
                    {
                        for (int i = 0; i < 5; i++)
                        {
                            bool bIsSelected = currMaterial == mehMaterialString[i];
                            if (ImGui::Selectable(mehMaterialString[i], bIsSelected))
                            {
                                currMaterial = mehMaterialString[i];
                                meshMaterial = (MeshComponent::MaterialType)i;
                            }
                            
                            if (bIsSelected)
                            {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        ImGui::EndCombo();
                    }
                }
                
                ImGui::TreePop();
            }
            ImGui::Separator();
        } // if (entity.HasComponent<MeshComponent>())
        
        if (entity.HasComponent<TransformComponent>())
        {
            auto& tc = entity.GetComponent<TransformComponent>();
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
            {
                auto [translation, rotationQuat, scale] = GlmMath::GetTransformDecomposition(tc);
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
        } // if (entity.HasComponent<TransformComponent>())
        
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
        } // if (entity.HasComponent<CameraComponent>())
        
        if (entity.HasComponent<LightComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(LightComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Light"))
            {
                auto& light = entity.GetComponent<LightComponent>().Light;
                
                ImGui::Columns(2);
                ImGui::Checkbox("IsAmbient", &light.LightFlag.IsAmbient);
                if (light.LightFlag.IsAmbient)
                {
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(-1);
                    ImGui::DragFloat3("##Ambient", &light.Ambient.x);
                    ImGui::Columns(1);
                    ImGui::PopItemWidth();
                }
                
                ImGui::Columns(2);
                ImGui::Checkbox("IsDiffuse", &light.LightFlag.IsDiffuse);
                if (light.LightFlag.IsDiffuse)
                {
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(-1);
                    ImGui::DragFloat3("##Diffuse", &light.Diffuse.x);
                    ImGui::Columns(1);
                    ImGui::PopItemWidth();
                }
                
                ImGui::Columns(2);
                ImGui::Checkbox("IsSpecular", &light.LightFlag.IsSpecular);
                if (light.LightFlag.IsSpecular)
                {
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(-1);
                    ImGui::DragFloat3("##Specular", &light.Specular.x);
                    ImGui::Columns(1);
                    ImGui::PopItemWidth();
                }
                
                ImGui::Separator();
                ImGui::Checkbox("IsAttenuation", &light.LightFlag.IsAttenuation);
                if (light.LightFlag.IsAttenuation)
                {
                    ImGui::Columns(2);
                    
                    ImGui::Text("Constant");
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(-1);
                    
                    ImGui::DragFloat("##Constant", &light.Constant, 0.01f, 0.0f, 10.0f);
                    ImGui::PopItemWidth();
                    ImGui::NextColumn();
                    
                    ImGui::Text("Linear");
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(-1);
                    
                    ImGui::DragFloat("##Linear", &light.Linear, 0.01f, 0.0f, 10.0f);
                    ImGui::PopItemWidth();
                    ImGui::NextColumn();
                    
                    ImGui::Text("Quadratic");
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(-1);
                    
                    ImGui::DragFloat("##Quadratic", &light.Quadratic, 0.01f, 0.0f, 10.0f);
                    ImGui::PopItemWidth();
                    ImGui::NextColumn();
                    
                    ImGui::Columns(1);
                }
                
                ImGui::Separator();
                ImGui::Checkbox("IsSpotLight", &light.LightFlag.IsSpotLight);
                if (light.LightFlag.IsSpotLight)
                {
                    ImGui::Columns(2);
                    
                    ImGui::Text("CutOff");
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(-1);
                    
                    ImGui::DragFloat("##CutOff", &light.CutOff, 1.0f, 0.0f, 360.0f);
                    ImGui::PopItemWidth();
                    ImGui::NextColumn();
                    
                    ImGui::Columns(1);
                }
                
                ImGui::TreePop();
            }
        } // if (entity.HasComponent<LightComponent>())
    }
    
}
