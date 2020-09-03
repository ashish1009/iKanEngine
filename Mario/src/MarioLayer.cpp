#include "MarioLayer.h"

namespace iKan {
    
    MarioLayer::MarioLayer()
    {
        
    }

    MarioLayer::~MarioLayer()
    {
        
    }

    void MarioLayer::OnAttach()
    {
        // TODO: Set the position of Shader later
        Renderer2D::AddShader("../../Mario/assets/shaders/Shader.glsl");
        
        FramebufferSpecification fbSpec;
        fbSpec.Width  = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbSpec);
        
        m_ActiveScene = std::make_shared<Scene>();
        
        m_SquareEntity = m_ActiveScene->CreateEntity("Green Square");
        m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 0.0f, 0.7f, 1.0f});
        
        m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
        m_CameraEntity.AddComponent<CameraComponent>();
    }
    
    void MarioLayer::OnDetach()
    {
        
    }
    
    void MarioLayer::OnEvent(Event& event)
    {
        
    }
                             
    void MarioLayer::OnUpdate(TimeStep timeStep)
    {
        /*
         This will render the 'old' sized framebuffer onto the 'new' sized ImGuiPanel
         and store the 'new' size in m_ViewportSize.
         The next frame will first resize the framebuffer as m_ViewportSize differs
         from m_Framebuffer.Width/Height before updating and rendering.
         This results in never rendering an empty (black) framebuffer.
         */
        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }
        
        Renderer2D::ResetStats();
        
        m_Framebuffer->Bind();
        
        RenderCommand::Clear();
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        
        m_ActiveScene->OnUpdate(timeStep);
        
        m_Framebuffer->Unbind();
    }
                              
    void MarioLayer::OnImguiRender()
    {
        // Note: Switch this to true to enable dockspace
        static bool               dockspaceOpen              = true;
        static bool               opt_fullscreen_persistant  = true;
        bool                      opt_fullscreen             = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags            = ImGuiDockNodeFlags_None;
        
        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        
        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();
        
        if (opt_fullscreen)
            ImGui::PopStyleVar(2);
        
        // ----------------------- DockSpace --------------------------------------------------------------
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit")) Application::Get().Close();
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        
        // --------------------------- Settings ------------------------------------------------
        ImGui::Begin("Settings");
        if (m_SquareEntity)
        {
            auto& tag = m_SquareEntity.GetComponent<TagComponent>().Tag;
            ImGui::Text("%s", tag.c_str());
            
            ImGui::Separator();
            auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
            ImGui::ColorEdit4("Color", glm::value_ptr(squareColor));
        }
        
        ImGui::Separator();
        ImGui::DragFloat3("Camera", glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]));
        
        auto& camera    = m_CameraEntity.GetComponent<CameraComponent>().Camera;
        float orthoSize = camera.GetOrthographicSize();
        if (ImGui::DragFloat("Second Camera Ortho Size", &orthoSize))
            camera.SetOrthographicSize(orthoSize);
        ImGui::End();
        
        //------------------------ Statistics -------------------------------------------------------------
        ImGui::Begin("Stats");
        auto stats = Renderer2D::GetStats();
        ImGui::Text("Draw Calls   : %d", stats.DrawCalls);
        ImGui::Text("Quad Counts  : %d", stats.QuadCount);
        ImGui::Text("Vertex Count : %d", stats.GetTotalVertexCount());
        ImGui::Text("Index Cound  : %d", stats.GetTotalIndexCount());
        ImGui::End();
        
        //------------------------ Frame Rates -------------------------------------------------------------
        ImGui::Begin("Frame Rate");
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        //------------------------ View Port ---------------------------------------------------------------
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");
        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
        
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        
        size_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
        ImGui::PopStyleVar();
        
        ImGui::End();
    }
                                   
}
