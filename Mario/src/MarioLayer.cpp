#include "MarioLayer.h"

#include <Entity/Background.h>
#include <Entity/MarioCamera.h>

namespace iKan {
        
    MarioLayer::MarioLayer()
    : Layer("Mario")
    {
    }

    MarioLayer::~MarioLayer()
    {
    }

    void MarioLayer::OnAttach()
    {
        ImGuiAPI::LightGreyBackground();
        
        /*
         Upload the Shader
         NOTE: Upload the shader before calling Renderer2D::Begin Scene
         */
        Renderer2D::SetShaader("../../Mario/assets/shaders/Shader.glsl");
 
        // Adding Scene
        m_Scene = Ref<Scene>::Create(Scene::SceneRendererType::_2D);
        
        // Frame Buffers
        FramebufferSpecification fbSpec;
        fbSpec.Width  = s_WindowWidth;
        fbSpec.Height = s_WindowHeight;
        
        m_FrameBuffer = Framebuffer::Create(fbSpec);
        
        // Creating all the entities
        m_Player = Player::Create(m_Scene);
        Background::Init(m_Scene);
        MarioCamera::Init(m_Scene);
        
        m_SceneHierarchyPannel.SetContext(m_Scene);
    }
    
    void MarioLayer::OnDetach()
    {
    }
    
    void MarioLayer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(IK_BIND_EVENT_FN(MarioLayer::OnKeyPressed));
        dispatcher.Dispatch<KeyReleasedEvent>(IK_BIND_EVENT_FN(MarioLayer::OnKeyReleased));
    }
    
    bool MarioLayer::OnKeyPressed(KeyPressedEvent& event)
    {
        return false;
    }
    
    bool MarioLayer::OnKeyReleased(KeyReleasedEvent& event)
    {
        m_Player->OnKeyReleased(event);
        return false;
    }
                                 
    void MarioLayer::OnUpdate(TimeStep timeStep)
    {
        // If resize the window call the update the Scene View port and Frame buffer should be resized
        if (FramebufferSpecification spec = m_FrameBuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        RendererStatistics::Reset();
        m_FrameBuffer->Bind();
        
        Renderer::Clear(Background::GetColor());
                    
        m_Scene->OnUpdate(timeStep);
        
        m_FrameBuffer->Unbind();
    }
                    
    void MarioLayer::OnImguiRender()
    {
        ImGuiAPI::EnableDcocking();
        // ----------------------- Menu Bar ---------------------------------------------------------------
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit")) Application::Get().Close();
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        
        // Stats 
        ImGuiAPI::StatsAndFrameRate((ImGuiRendererType)m_Scene->GetRendererType());
        ImGuiAPI::RendererVersion();
        
        m_SceneHierarchyPannel.OnImguiender();
        
        // ----------------------- Setings ----------------------------------------------------------------
        ImGui::Begin("Setting");
        Background::ImGuiRender();
        m_Player->ImGuiRender();
        ImGui::End();
        
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
            
        // Ending of Docking egining
        ImGuiAPI::EndDocking();
    }
                                    
}
