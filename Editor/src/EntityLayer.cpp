#include "EntityLayer.h"

namespace iKan {
        
    EntityLayer::EntityLayer()
    {
        
    }
    
    EntityLayer::~EntityLayer()
    {
    }
    
    void EntityLayer::OnAttach()
    {
        SceneRenderer::SetShaader("../../Editor/assets/shaders/CommonShader.glsl");
        
        ImGuiAPI::LightGreyBackground();
        
        FramebufferSpecification specs;
        specs.Width  = s_WindowWidth;
        specs.Height = s_WindowWidth;
        
        m_FrameBuffer = Framebuffer::Create(specs);
        
        m_ActiveScene = Ref<Scene>::Create(Scene::SceneRendererType::_3D);
        
        m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
        m_CameraEntity.AddComponent<CameraComponent>();
        
        Entity cubeEntity = m_ActiveScene->CreateEntity("Cube");
        cubeEntity.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 0.0f, 0.7f, 1.0f});
        
        m_SceneHierarchyPannel.SetContext(m_ActiveScene);
    }
    
    void EntityLayer::OnDetach()
    {
    }
    
    void EntityLayer::OnEvent(Event& event)
    {
    }
    
    void EntityLayer::OnUpdate(TimeStep timeStep)
    {
        // If resize the window call the update the Scene View port and Frame buffer should be resized
        if (FramebufferSpecification spec = m_FrameBuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }
        
        RendererStatistics::Reset();
        m_FrameBuffer->Bind();

        Renderer::Clear({ 0.15f, 0.15f, 0.15f, 1.0f });
        m_ActiveScene->OnUpdate(timeStep);
        
        m_FrameBuffer->Unbind();
    }
    
    void EntityLayer::OnImguiRender()
    {
        ImGuiAPI::EnableDcocking();
        
        //------------------------ Stats and Version  ------------------------------------------------------
        ImGuiAPI::StatsAndFrameRate();
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
