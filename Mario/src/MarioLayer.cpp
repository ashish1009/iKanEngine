#include "MarioLayer.h"

namespace iKan {
    
    static glm::vec4 s_BgColor = { 0.1f, 0.1f, 0.1f, 1.0f };
    
    static void HelpMarker(const char* desc)
    {
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    MarioLayer::MarioLayer()
    {
    }

    MarioLayer::~MarioLayer()
    {
    }

    void MarioLayer::OnAttach()
    {
        FramebufferSpecification fbSpec;
        fbSpec.Width  = iKan::s_WindowWidth;
        fbSpec.Height = iKan::s_WindowHeight;
        
        m_FrameBuffer = Framebuffer::Create(fbSpec);
    }
    
    void MarioLayer::OnDetach()
    {
    }
    
    void MarioLayer::OnEvent(Event& event)
    {
    }
                             
    void MarioLayer::OnUpdate(TimeStep timeStep)
    {
        m_FrameBuffer->Bind();
        
        RenderCommand::Clear();
        RenderCommand::SetClearColor(s_BgColor);
        
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
                ImGuiColorEditFlags miscFlags = (alphaHalfPreview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alphaPreview ? ImGuiColorEditFlags_AlphaPreview : 0));
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
                
                static int pickerMode = 0;
                ImGui::Combo("Picker Mode", &pickerMode, "Auto/Current\0Hue bar + SV rect\0Hue wheel + SV triangle\0");
                ImGui::SameLine(); HelpMarker("User can right-click the picker to change mode.");

                ImGuiColorEditFlags flags = miscFlags | ImGuiColorEditFlags_PickerHueWheel;

                if (!sidePreview)     flags |= ImGuiColorEditFlags_NoSidePreview;
                if (pickerMode == 1)  flags |= ImGuiColorEditFlags_PickerHueBar;
                if (pickerMode == 2)  flags |= ImGuiColorEditFlags_PickerHueWheel;
                ImGui::ColorPicker4("Back Ground##4", (float*)&color, flags, refColor ? &refColorValue.x : NULL);
                                
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
        ImGui::ShowDemoWindow();
        
        // Ending of Docking egining
        ImGui::End();
    }
                                    
}
