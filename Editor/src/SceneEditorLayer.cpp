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
        specs.Width  = s_WindowWidth;
        specs.Height = s_WindowWidth;
        
        m_FrameBuffer = Framebuffer::Create(specs);
        
        m_ActiveScene = Ref<Scene>::Create();
                        
        m_SceneHierarchyPannel.SetContext(m_ActiveScene);
    }
    
    void SceneEditor::OnDetach()
    {
    }
    
    void SceneEditor::OnUpdate(TimeStep timeStep)
    {
        // If resize the window call the update the Scene View port and Frame buffer should be resized
        if (FramebufferSpecification spec = m_FrameBuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_EditorCamera.SetViewportSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }
        
        m_EditorCamera.OnUpdate(timeStep);
        
        RendererStatistics::Reset();
        m_FrameBuffer->Bind();
        
        Renderer::Clear(m_BgColor);
        
        m_ActiveScene->OnRenderEditor(timeStep, m_EditorCamera);
        
        m_FrameBuffer->Unbind();
    }
    
    void SceneEditor::OnImguiRender()
    {
        ImGuiAPI::EnableDcocking();
        
        //------------------------ Menu Bar  ------------------------------------------------------
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
                
                ImGui::EndMenu();
            }
            
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
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        
        //------------------------ Stats and Version  ------------------------------------------------------
        ImGuiAPI::FrameRate();
        ImGuiAPI::RendererStats();
        ImGuiAPI::RendererVersion();
        
        //------------------------ SceneHierarchy Pannel  --------------------------------------------------
        m_SceneHierarchyPannel.OnImguiender();
        
        //------------------------ View Port ---------------------------------------------------------------
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");
        
        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
        
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        
        size_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
        ImGui::PopStyleVar();
        
        ImGuiAPI::EndDocking();
    }
    
    void SceneEditor::OnEvent(Event& event)
    {
        m_EditorCamera.OnEvent(event);
        
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(IK_BIND_EVENT_FN(SceneEditor::OnKeyPressed));
    }
    
    bool SceneEditor::OnKeyPressed(KeyPressedEvent& event)
    {
        // Shortcuts
        if (event.GetRepeatCount() > 0)
            return false;
        
        bool cmd   = Input::IsKeyPressed(Key::LeftSuper) || Input::IsKeyPressed(Key::RightSuper);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
        switch (event.GetKeyCode())
        {
            case Key::N:
            {
                if (cmd)
                    NewScene();
                break;
            }
            case Key::O:
            {
                if (cmd)
                    OpenScene();
                break;
            }
            case Key::S:
            {
                if (cmd && shift)
                    SaveSceneAs();
                break;
            }
                
            default:
                break;
        }
        return false;
    }
    
    void SceneEditor::NewScene()
    {
        m_ActiveScene = Ref<Scene>::Create();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPannel.SetContext(m_ActiveScene);
    }
    
    void SceneEditor::OpenScene()
    {
        std::string filepath = "../../Editor/assets/scene/Example.iKan";
        if (!filepath.empty())
        {
            m_ActiveScene = Ref<Scene>::Create();
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
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
