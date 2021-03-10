#include "SceneEditorLayer.h"

namespace iKan {
    
    SceneEditor::SceneEditor()
    : m_EditorCamera(glm::radians(45.0f), 1800.0f/800.0f, 0.01f, 10000.0f)
    {
    }
    
    SceneEditor::~SceneEditor()
    {
    }
    
    void SceneEditor::OnAttach()
    {
        ImGuiAPI::SetDarkThemeColors();

        FramebufferSpecification specs;
        specs.Attachments = { FbTextureFormat::RGBA8, FbTextureFormat::Depth, FbTextureFormat::R32I};
        specs.Width       = s_WindowWidth;
        specs.Height      = s_WindowWidth;
        
        m_Viewport.FrameBuffer = Framebuffer::Create(specs);

        m_ActiveScene  = Ref<Scene>::Create();
        m_SceneHierarchyPannel.SetContext(m_ActiveScene);
    }
    
    void SceneEditor::OnDetach()
    {
    }

    void SceneEditor::OnUpdate(TimeStep timeStep)
    {
        // If resize the window call the update the Scene View port and Frame buffer should be resized
        if (FramebufferSpecification spec = m_Viewport.FrameBuffer->GetSpecification();
            m_Viewport.Size.x > 0.0f && m_Viewport.Size.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_Viewport.Size.x || spec.Height != m_Viewport.Size.y))
        {
            m_Viewport.FrameBuffer->Resize((uint32_t)m_Viewport.Size.x, (uint32_t)m_Viewport.Size.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_Viewport.Size.x, (uint32_t)m_Viewport.Size.y);
            m_EditorCamera.SetViewportSize((uint32_t)m_Viewport.Size.x, (uint32_t)m_Viewport.Size.y);
        }
        m_EditorCamera.OnUpdate(timeStep);
        RendererStatistics::Reset();
        
        m_Viewport.FrameBuffer->Bind();
        {
            Renderer::Clear(m_BgColor);
            m_ActiveScene->OnUpdateEditor(timeStep, m_EditorCamera);
            
            UpdateViewportMousePos();
            UpdateHoveredEntity();
        }
        m_Viewport.FrameBuffer->Unbind();
    }
    
    void SceneEditor::UpdateHoveredEntity()
    {
        if (m_Viewport.MouseX >= 0 && m_Viewport.MouseY >= 0 && m_Viewport.MouseX <= m_Viewport.Width && m_Viewport.MouseY <= m_Viewport.Height )
        {
            int32_t ID = m_ActiveScene->Pixel(m_Viewport.MouseX, m_Viewport.MouseY);
            // TODO:: remove entt::entity
            m_HoveredEntity = (ID == 1028443341) ? Entity() : Entity((entt::entity)ID, m_ActiveScene.Raw());
        }
    }
    
    void SceneEditor::UpdateViewportMousePos()
    {
        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_Viewport.Bounds[0].x;
        my -= m_Viewport.Bounds[0].y;

        m_Viewport.Height = m_Viewport.Bounds[1].y - m_Viewport.Bounds[0].y;
        m_Viewport.Width  = m_Viewport.Bounds[1].x - m_Viewport.Bounds[0].x;

        my = m_Viewport.Height - my;

        m_Viewport.MouseX = (int32_t)mx;
        m_Viewport.MouseY = (int32_t)my;
    }
    
    void SceneEditor::OnImguiRender()
    {
        ImGuiAPI::EnableDcocking();
        ShowMenu();
        
        ImGuiAPI::FrameRate();
        ImGuiAPI::RendererStats();
        ImGuiAPI::RendererVersion();

        PrintHoveredEntity();

        m_SceneHierarchyPannel.OnImguiender();
        
        //------------------------ View Port ---------------------------------------------------------------
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");

        auto viewportOffet = ImGui::GetCursorPos();

        m_Viewport.Focused = ImGui::IsWindowFocused();
        m_Viewport.Hovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_Viewport.Focused && !m_Viewport.Hovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_Viewport.Size = { viewportPanelSize.x, viewportPanelSize.y };
        
        size_t textureID = m_Viewport.FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)textureID, ImVec2{ m_Viewport.Size.x, m_Viewport.Size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        auto windowSize = ImGui::GetWindowSize();
        ImVec2 minBound = ImGui::GetWindowPos();
        minBound.x += viewportOffet.x;
        minBound.y += viewportOffet.y;

        ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
        m_Viewport.Bounds[0] = { minBound.x, minBound.y };
        m_Viewport.Bounds[1] = { maxBound.x, maxBound.y };

        // Gizmos
        Entity selectedEntity = m_SceneHierarchyPannel.GetSelectedEntity();
        if (selectedEntity && m_GizmoType != -1)
        {
            ImGuizmo::SetOrthographic(true);
            ImGuizmo::SetDrawlist();

            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            // Runtime camera from entity
#if 0
            auto cameraEntity = m_ActiveScene->GetMainCameraEntity();
            const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
            const glm::mat4& cameraProjection = camera.GetProjection();
            glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());
#endif
            // Editor camera
            const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
            glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

            // Entity transform
            auto& tc = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = tc.GetTransform();

            // Snapping
            bool snap = Input::IsKeyPressed(KeyCode::LeftControl);
            float snapValue = 0.5f; // Snap to 0.5m for translation/scale
                                    // Snap to 45 degrees for rotation
            if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                snapValue = 45.0f;

            float snapValues[3] = { snapValue, snapValue, snapValue };

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                                 (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                                 nullptr, snap ? snapValues : nullptr);

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 translation, rotation, scale;
                Math::DecomposeTransform(transform, translation, rotation, scale);

                glm::vec3 deltaRotation = rotation - tc.Rotation;
                tc.Translation = translation;
                tc.Rotation += deltaRotation;
                tc.Scale = scale;
            }
        }

        ImGui::End();
        ImGui::PopStyleVar();
        
        ImGuiAPI::EndDocking();
    }
    
    void SceneEditor::PrintHoveredEntity()
    {
        ImGui::Begin("Selected Entity");
        std::string entityName = "NULL";
        if ((entt::entity)m_HoveredEntity != entt::null)
        {
            entityName = m_HoveredEntity.GetComponent<TagComponent>().Tag;
            ImGui::Text("Hovered Entity -> ID: %d, Tag: %s", (uint32_t)m_HoveredEntity, entityName.c_str());
        }
        ImGui::End();
    }
    
    void SceneEditor::ShowMenu()
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "Cmd+N"))
                    NewScene();
                
                if (ImGui::MenuItem("Save", "Cmd+S"))
                    SaveSceneAs();
                    
                if (ImGui::MenuItem("Open", "Cmd+S"))
                    OpenScene();
                
                if (ImGui::MenuItem("Exit"))
                    Application::Get().Close();
                
                ImGui::EndMenu(); // ImGui::BeginMenu("File")
            } // if (ImGui::BeginMenuBar())
            
            if (ImGui::BeginMenu("Properties"))
            {
                if (ImGui::BeginMenu("Theme"))
                {
                    if (ImGui::MenuItem("Light"))
                    {
                        ImGuiAPI::SetLightThemeColors();
                        m_BgColor = { 0.8f, 0.8f, 0.8f, 1.0f };
                    }
                    if (ImGui::MenuItem("Dark"))
                    {
                        ImGuiAPI::SetDarkThemeColors();
                        m_BgColor = { 0.1f, 0.1f, 0.1f, 1.0f };
                    }
                    if (ImGui::MenuItem("Grey"))
                    {
                        ImGuiAPI::SetGreyThemeColors();
                        m_BgColor = { 0.25f, 0.25f, 0.25f, 1.0f };
                    }
                    ImGui::EndMenu(); // ImGui::BeginMenu("Theme")
                }
                ImGui::EndMenu(); // ImGui::BeginMenu("Properties")
            } // if (ImGui::BeginMenu("Properties"))
            ImGui::EndMenuBar(); // ImGui::BeginMenuBar()
        }
    }
    
    void SceneEditor::OnEvent(Event& event)
    {
        m_EditorCamera.OnEvent(event);
        
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(IK_BIND_EVENT_FN(SceneEditor::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(IK_BIND_EVENT_FN(SceneEditor::OnMouseButtonPressed));
    }

    bool SceneEditor::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        if (e.GetMouseButton() == MouseCode::ButtonLeft && !ImGuizmo::IsOver() && !Input::IsKeyPressed(KeyCode::LeftAlt))
            if (m_Viewport.MouseX >= 0 && m_Viewport.MouseY >= 0 && m_Viewport.MouseX <= m_Viewport.Width && m_Viewport.MouseY <= m_Viewport.Height )
                m_SceneHierarchyPannel.SetSelectedEntity(m_HoveredEntity);
        return false;
    }
    
    bool SceneEditor::OnKeyPressed(KeyPressedEvent& event)
    {
        // Shortcuts
        if (event.GetRepeatCount() > 0)
            return false;
        
        bool cmd   = Input::IsKeyPressed(KeyCode::LeftSuper) || Input::IsKeyPressed(KeyCode::RightSuper);
        bool shift = Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift);
        switch (event.GetKeyCode())
        {
            case KeyCode::N:
            {
                if (cmd)
                    NewScene();
                break;
            }
            case KeyCode::O:
            {
                if (cmd)
                    OpenScene();
                break;
            }
            case KeyCode::S:
            {
                if (cmd && shift)
                    SaveSceneAs();
                break;
            }

                // Gizmos
            case KeyCode::Q:
                m_GizmoType = -1;
                break;
            case KeyCode::W:
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                break;
            case KeyCode::E:
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
                break;
            case KeyCode::R:
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
                break;
                
            default:
                break;
        }
        return false;
    }
    
    void SceneEditor::NewScene()
    {
        m_ActiveScene = Ref<Scene>::Create();
        m_ActiveScene->OnViewportResize((uint32_t)m_Viewport.Size.x, (uint32_t)m_Viewport.Size.y);
        m_SceneHierarchyPannel.SetContext(m_ActiveScene);
    }
    
    void SceneEditor::OpenScene()
    {
        std::string filepath = "../../Editor/assets/scene/Example.iKan";
        if (!filepath.empty())
        {
            m_ActiveScene = Ref<Scene>::Create();
            m_ActiveScene->OnViewportResize((uint32_t)m_Viewport.Size.x, (uint32_t)m_Viewport.Size.y);
            m_SceneHierarchyPannel.SetContext(m_ActiveScene);
            
            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(filepath);
        }
    }
    
    void SceneEditor::SaveSceneAs()
    {
        std::string filepath = "../../Editor/assets/scene/Example.iKan";
        if (!filepath.empty())
        {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(filepath);
        }
    }
    
}
