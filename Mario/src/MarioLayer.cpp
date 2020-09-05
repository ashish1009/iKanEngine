#include "MarioLayer.h"

namespace iKan {
    
    static glm::vec4 s_BgColor = { 0.3f, 0.1f, 0.6f, 1.0f };
    
    MarioLayer::MarioLayer()
    {
    }

    MarioLayer::~MarioLayer()
    {
    }

    void MarioLayer::OnAttach()
    {
        /*
         Upload the Shader
         NOTE: Upload the shader before calling Renderer2D::Begin Scene
         */
        Renderer2D::SetShaader("../../Mario/assets/shaders/Shader.glsl");
 
        // Frame Buffers
        FramebufferSpecification fbSpec;
        fbSpec.Width  = iKan::s_WindowWidth;
        fbSpec.Height = iKan::s_WindowHeight;
        
        m_FrameBuffer = Framebuffer::Create(fbSpec);
        
        // Texture tile
        m_TileSpriteSheet = Texture::Create("../../Mario/assets/Resources/Graphics/Tile.png");
        m_TextureMap['G'] = SubTexture::CreateFromCoords(m_TileSpriteSheet, { 0, 21 });
        
        // Adding Scene
        m_Scene = std::make_shared<Scene>();
        
        m_BackgroundEntity = m_Scene->CreateEntity("Background");
        m_BackgroundEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.2f, 0.3f, 0.4f, 1.0f));
        
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)) * glm::scale(glm::mat4(1.0f), { 2.0f, 1.0f, 1.0f });
        
        m_BackgroundEntity.GetComponent<TransformComponent>().Transform = transform;
        
        m_PrimaryCameraEntity = m_Scene->CreateEntity("Primary Camera");
        m_PrimaryCameraEntity.AddComponent<CameraComponent>();
    }
    
    void MarioLayer::OnDetach()
    {
    }
    
    void MarioLayer::OnEvent(Event& event)
    {
    }
                             
    void MarioLayer::OnUpdate(TimeStep timeStep)
    {
        if (FramebufferSpecification spec = m_FrameBuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        Renderer2D::ResetStats();
        m_FrameBuffer->Bind();
        
        RenderCommand::Clear();
        RenderCommand::SetClearColor(s_BgColor);
        
        Camera camera(glm::mat4(1.0f));
        Renderer2D::BeginScene(camera, glm::mat4(1.0f));
    
        if (m_TextureMap.find('G') != m_TextureMap.end())
        {
            std::shared_ptr<SubTexture> subTexture  = m_TextureMap['G'];
            Renderer2D::DrawQuad({ 0.0f, 0.0f }, subTexture->GetSpriteSize(), subTexture);
        }
        
        Renderer2D::EndScene();
        
        m_Scene->OnUpdate(timeStep);
        
        m_FrameBuffer->Unbind();
    }
                    
    void MarioLayer::OnImguiRender()
    {
        // Note: Switch this to true to enable dockspace
        static bool               dockspaceOpen              = false;
        static bool               opt_fullscreen_persistant  = true;
        bool                      opt_fullscreen             = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags            = ImGuiDockNodeFlags_None;
        
        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;// | ImGuiWindowFlags_NoDocking;
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
        ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
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
        // ----------------------- Setings ----------------------------------------------------------------
        ImGui::Begin("Setting");
        if (ImGui::CollapsingHeader("BackGround"))
        {
            if (ImGui::TreeNode("Color"))
            {
                static ImVec4 color         = ImVec4(s_BgColor.r, s_BgColor.g, s_BgColor.b, s_BgColor.a);
                static ImVec4 refColorValue = color;
                
                static bool alphaPreview = true, alphaHalfPreview = true;
                ImGui::Checkbox("Alpha", &alphaPreview);  ImGui::SameLine(); ImGui::Checkbox("Half Alpha", &alphaHalfPreview);
                ImGuiColorEditFlags miscFlags = ImGuiColorEditFlags_PickerHueWheel | (alphaHalfPreview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alphaPreview ? ImGuiColorEditFlags_AlphaPreview : 0));
                if (alphaPreview || alphaHalfPreview) miscFlags |= ImGuiColorEditFlags_AlphaBar; else miscFlags |= ImGuiColorEditFlags_NoAlpha;

                static bool sidePreview = true, refColor = false;
                ImGui::Checkbox("Side Preview", &sidePreview);
                if (sidePreview)
                {
                    ImGui::SameLine();
                    ImGui::Checkbox("Ref Color", &refColor);
                    if (refColor)
                    {
                        ImGui::SameLine();
                        ImGui::ColorEdit4("##RefColor", &refColorValue.x, ImGuiColorEditFlags_NoInputs | miscFlags);
                    }
                }
                if (!sidePreview)
                    miscFlags |= ImGuiColorEditFlags_NoSidePreview;
            
                ImGui::ColorPicker4("Back Ground##4", (float*)&color, miscFlags, refColor ? &refColorValue.x : NULL);
                ImGui::TreePop();
                
                s_BgColor = { color.x, color.y, color.z, color.w };
            }
        }
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
            
        Renderer2D::ImguiStatsAnfFrameRate();
        
        // Only for Demo
//        ImGui::ShowDemoWindow();
        
        // Ending of Docking egining
        ImGui::End();
    }
                                    
}
