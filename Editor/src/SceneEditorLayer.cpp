#include "SceneEditorLayer.h"

namespace iKan {
    
    SceneCamera camera;

    SceneEditor::SceneEditor()
    : m_EditorCamera(glm::radians(45.0f), 1800.0f/800.0f, 0.01f, 10000.0f)
    {
    }
    
    SceneEditor::~SceneEditor()
    {
    }
    
    void SceneEditor::OnAttach()
    {
        ImGuiAPI::LightGreyBackground();
        
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
    
    void SceneEditor::OnEvent(Event& event)
    {
        m_EditorCamera.OnEvent(event);
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
        
        Renderer::Clear({ 0.2, 0.2, 0.2, 1.0f });
        
        camera.SetViewportSize(1600.0f, 800.0f);
        
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
                if (ImGui::MenuItem("Exit")) Application::Get().Close();
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
    
}
