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

            if (ImGui::MenuItem("Light"))
            {
                m_SelectionContext.AddComponent<LightComponent>();
                ImGui::CloseCurrentPopup();
            }
            
            if (ImGui::MenuItem("Mesh"))
            {
                m_SelectionContext.AddComponent<MeshComponent>();
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::EndPopup();
        }
        
        ImGui::PopItemWidth();
        
        DrawComponent<TransformComponent>("Transform", entity, [](auto& tc)
                                          {
            Property("Translation", tc.Translation, 0.25f);
            glm::vec3 rotation = glm::degrees(tc.Rotation);
            Property("Rotation", rotation, 0.25f);
            tc.Rotation = glm::radians(rotation);
            Property("Scale", tc.Scale, 0.25f, 1.0f);
        });
        
        DrawComponent<CameraComponent>("Camera", entity, [](auto& cc)
                                       {
            auto& camera = cc.Camera;
            
            Property("Primary", cc.Primary);
            ImGui::Separator();

            {
                ImGui::Columns(2);
                ImGui::Text("Projection Type");
                ImGui::NextColumn();
                ImGui::PushItemWidth(-1);
                
                const std::vector <const char*> projectionTypeSTring = { "Projection", "Orthographic" };
                const char* currentProjectionType = projectionTypeSTring[(int32_t)camera.GetProjectionType()];
                if (ImGui::BeginCombo("##Projection", currentProjectionType))
                {
                    for (int i = 0; i < projectionTypeSTring.size(); i++)
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
                
                Property("Near", camera.m_PerspectiveNear, 0.001f, 0.01f);
                Property("Far", camera.m_PerspectiveFar, 10.0f, 1000.0f);
            }
            
            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
            {
                Property("Size", camera.m_OrthographicSize, 0.1f, 10.0f);
                Property("Near", camera.m_OrthographicNear, 0.1f, -1.0f);
                Property("Far", camera.m_OrthographicFar, 0.1f, 1.0f);
                
                ImGui::Separator();
                Property("Fixed Aspect Ratio", cc.FixedAspectRatio);
            }
        });
        
        DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& src)
                                               {
            ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));
            if (src.Texture || src.SubTexComp)
                ImGuiAPI::Counter("Tiling Factor", src.TilingFactor);
            ImGui::Separator();
            
            ImGui::Columns(2);
            ImGui::SetColumnWidth(0, 100);
            ImGui::SetColumnWidth(1, 300);
            ImGui::Text("File Path");
            ImGui::NextColumn();
            ImGui::PushItemWidth(-1);
            {
                std::string currentTexture;
                {
                    if (src.Texture)
                    {
                        // Extract the name of current Mesh
                        auto path      = src.Texture->GetfilePath();
                        auto lastSlash = path.find_last_of("/\\");
                        lastSlash      = lastSlash == std::string::npos ? 0 : lastSlash + 1;
                        
                        auto lastDot = path.find_last_of('.');
                        auto count   = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
                        
                        currentTexture = path.substr(lastSlash, count);
                        ImGui::InputText("##Texturefilepath", (char*)path.c_str(), 256, ImGuiInputTextFlags_ReadOnly);
                    }
                    else
                    {
                        currentTexture = "Null";
                        ImGui::InputText("##Texturefilepath", (char*)"Null", 256, ImGuiInputTextFlags_ReadOnly);
                        
                    }
                    ImGui::PopItemWidth();
                }
                
                ImGui::Columns(1);
                
                ImGui::Columns(2);
                ImGui::Text("Available Textures");
                ImGui::NextColumn();
                ImGui::PushItemWidth(-1);
                
                const std::vector<const char*> availabelTextureString = { "checkerboard", "container", "grass", "marble", "matrix", "metal", "smile", "window", "wood" };
                if (ImGui::BeginCombo("##Type", currentTexture.c_str()))
                {
                    for (int i = 0; i < availabelTextureString.size(); i++)
                    {
                        bool bIsSelected = currentTexture == availabelTextureString[i];
                        if (ImGui::Selectable(availabelTextureString[i], bIsSelected))
                        {
                            currentTexture = availabelTextureString[i];
                            // TODO: for now path is hard coded will fix this in future
                            // Make sure file name is same as the foler name
                            std::string filePath = "/Users/ashish./iKan/GitHub/iKanEngine/Editor/assets/resources/texture/" + currentTexture + ".png";
                            src.Texture = Texture::Create(filePath);
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
        });
        
        DrawComponent<MeshComponent>("Mesh", entity, [](auto& mc)
                                     {
            ImGui::Columns(2);
            ImGui::SetColumnWidth(0, 100);
            ImGui::SetColumnWidth(1, 300);
            ImGui::Text("File Path");
            ImGui::NextColumn();
            ImGui::PushItemWidth(-1);
            ImGui::Columns(1);

            // TODO: Provide the option of Mesh we have May be later use file open dialog box
            ImGui::Separator();
            {
                ImGui::Columns(2);
                ImGui::Text("Available Mesh");
                ImGui::NextColumn();
                ImGui::PushItemWidth(-1);
                
                std::string currentMeshType;
                {
                    if (mc.Mesh)
                    {
                        // Extract the name of current Mesh
                        auto path      = mc.Mesh->GetFilepath();
                        auto lastSlash = path.find_last_of("/\\");
                        lastSlash      = lastSlash == std::string::npos ? 0 : lastSlash + 1;
                        
                        auto lastDot = path.find_last_of('.');
                        auto count   = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
                        
                        currentMeshType = path.substr(lastSlash, count);
                        ImGui::InputText("##meshfilepath", (char*)path.c_str(), 256, ImGuiInputTextFlags_ReadOnly);
                    }
                    else
                    {
                        currentMeshType = "Null";
                        ImGui::InputText("##meshfilepath", (char*)"Null", 256, ImGuiInputTextFlags_ReadOnly);

                    }
                    ImGui::PopItemWidth();
                }

                const std::vector<const char*> availabelMeshString = { "backpack", "GroundPlane", "Sphere" };
                if (ImGui::BeginCombo("##Type", currentMeshType.c_str()))
                {
                    for (int i = 0; i < availabelMeshString.size(); i++)
                    {
                        bool bIsSelected = currentMeshType == availabelMeshString[i];
                        if (ImGui::Selectable(availabelMeshString[i], bIsSelected))
                        {
                            currentMeshType = availabelMeshString[i];
                            // TODO: for now path is hard coded will fix this in future
                            // Make sure file name is same as the foler name
                            std::string filePath = "/Users/ashish./iKan/GitHub/iKanEngine/Editor/assets/resources/objects/" + currentMeshType + "/" + currentMeshType + ".obj";
                            mc.Mesh = Ref<Mesh>::Create(filePath);
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
        });
        
        DrawComponent<LightComponent>("Light", entity, [](auto& lc)
                                               {
            ImGui::Columns(3);
            ImGui::SetColumnWidth(0, 120);
            ImGui::SetColumnWidth(1, 80);
            ImGui::SetColumnWidth(2, 150);

            ImGui::Checkbox("Is Scene Light", &lc.IsLight);
            ImGui::NextColumn();
            if (lc.IsLight)
            {
                auto& light = lc.Light;
                ImGui::Separator();
                {
                    ImGui::Text("Light Type");
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(-1);
                    
                    const std::vector<const char*> lightypeSTring = { "Direction", "Point", "Spot" };
                    const char* currentlightType = lightypeSTring[(int32_t)light.GetType()];
                    if (ImGui::BeginCombo("##Type", currentlightType))
                    {
                        for (int i = 0; i < lightypeSTring.size(); i++)
                        {
                            bool bIsSelected = currentlightType == lightypeSTring[i];
                            if (ImGui::Selectable(lightypeSTring[i], bIsSelected))
                            {
                                currentlightType = lightypeSTring[i];
                                light.SetType((SceneLight::LightType)i);
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

                Property("Ambient", &light.m_IsAmbient, light.m_Ambient, 0.1f, 0.2f);
                Property("Diffuse", &light.m_IsDiffuse, light.m_Diffuse, 0.1f, 0.5f);
                Property("Specular", &light.m_IsSpecular, light.m_Specular, 0.1f, 1.0f);
                
                ImGui::Separator();
                Property("Blinn", light.m_IsBlinn);
                ImGui::Separator();
                
                if (light.GetType() == SceneLight::LightType::Point)
                {
                    Property("Linear", light.m_Linear, 0.001, 0.09);
                    Property("Quadratic", light.m_Quadratic, 0.001, 0.032f);
                    Property("Constant", light.m_Constant, 0.001, 1.0f);
                }
                
                if (light.GetType() == SceneLight::LightType::Spot)
                {
                    Property("Cutoff", light.m_CutOff, 0.01, 12.5f);
                    Property("Outer Cutoff", light.m_OuterCutOff, 0.01, 15.0f);
                }

            }
        });
        
    }

}


