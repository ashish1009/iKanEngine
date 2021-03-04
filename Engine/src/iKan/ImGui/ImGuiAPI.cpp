// ******************************************************************************
//   File    : ImGuiAPI.cpp
//   Project : i-Kan : ImGui
//
//   Created by Ashish
// ******************************************************************************

#include <iKan/ImGui/ImGuiAPI.h>
#include <imgui_internal.h>
#include <iKan/Renderer/API/RendererAPI.h>
#include <iKan/Renderer/API/RendererStats.h>

namespace iKan {
    
    // ******************************************************************************
    // Renderer Version
    // ******************************************************************************
    void ImGuiAPI::RendererVersion(bool *pIsOpen)
    {
        ImGui::Begin("Renderer", pIsOpen);
        auto& caps = RendererAPI::GetCapabilities();

        ImGui::PushID("Renderer");
        
        ImGui::Columns(3);

        ImGui::SetColumnWidth(0, 110);
        ImGui::Text("Vendor : %s", caps.Vendor.c_str());
        ImGui::NextColumn();
        
        ImGui::SetColumnWidth(1, 220);
        ImGui::Text("Renderer : %s", caps.Renderer.c_str());
        ImGui::NextColumn();

        ImGui::SetColumnWidth(2, 150);
        ImGui::Text("Version : %s", caps.Version.c_str());
        ImGui::NextColumn();

        ImGui::Columns(1);
        
        ImGui::PopID();
        
        ImGui::End();
    }
    
    // ******************************************************************************
    // Frame Rate
    // ******************************************************************************
    void ImGuiAPI::FrameRate(bool *pIsOpen)
    {
        ImGui::Begin("Frame Rate", pIsOpen);
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
    
    // ******************************************************************************
    // renderer Stats
    // ******************************************************************************
    void ImGuiAPI::RendererStats(bool *pIsOpen)
    {
        //------------------------ Statistics -------------------------------------------------------------
        ImGui::Begin("Stats", pIsOpen);
        
        ImGui::PushID("Renderer");
        
        ImGui::Columns(4);
        
        ImGui::SetColumnWidth(0, 120);
        ImGui::Text("Draw Calls : %d", RendererStatistics::DrawCalls);
        ImGui::NextColumn();
        
        ImGui::SetColumnWidth(1, 140);
        ImGui::Text("Vertex Counts : %d", RendererStatistics::VertexCount);
        ImGui::NextColumn();
        
        ImGui::SetColumnWidth(2, 140);
        ImGui::Text("Index Counts : %d", RendererStatistics::IndexCount);
        ImGui::NextColumn();
        
        ImGui::SetColumnWidth(3, 120);
        ImGui::Text("Texture Counts : %d", RendererStatistics::TextureCount);
        
        ImGui::Columns(1);
        
        ImGui::PopID();
        
        ImGui::End();
    }
    
    // ******************************************************************************
    // Integer counter
    // ******************************************************************************
    void ImGuiAPI::Counter(const std::string& name, uint32_t& counter)
    {
        ImGui::Text(name.c_str());
        ImGui::SameLine();
        
        // Arrow buttons with Repeater
        float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
        
        ImGui::PushButtonRepeat(true);
        if (ImGui::ArrowButton("##left", ImGuiDir_Left))
            counter--;
        
        ImGui::SameLine(0.0f, spacing);
        if (ImGui::ArrowButton("##right", ImGuiDir_Right))
            counter++;
        
        ImGui::PopButtonRepeat();
        ImGui::SameLine();
        ImGui::Text("%d", counter);
    }

    // ******************************************************************************
    // Float counter
    // ******************************************************************************
    void ImGuiAPI::Counter(const std::string& name, float& value)
    {
        uint32_t counter = (uint32_t)value;
        Counter(name, counter);
        value = (uint32_t)counter;
    }
    
    // ******************************************************************************
    // Color Edit
    // ******************************************************************************
    void ImGuiAPI::ColorEdit(glm::vec4& colorRef)
    {
        static ImVec4 color         = ImVec4(colorRef.r, colorRef.g, colorRef.b, colorRef.a);
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
        
        colorRef = { color.x, color.y, color.z, color.w };
    }
    
    // ******************************************************************************
    // Enabling Docking
    // ******************************************************************************
    void ImGuiAPI::EnableDcocking()
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
        
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        float minWinSizeY = style.WindowMinSize.y;
        style.WindowMinSize.x = 150.0f;
        style.WindowMinSize.y = 40.0f;
        
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        
        style.WindowMinSize.x = minWinSizeX;
        style.WindowMinSize.y = minWinSizeY;
    }
    
    // ******************************************************************************
    // Close Docking
    // ******************************************************************************
    void ImGuiAPI::EndDocking()
    {
        ImGui::End();
    }
    
    // ******************************************************************************
    // Grey theme
    // ******************************************************************************
    void ImGuiAPI::SetGreyThemeColors()
    {
        IK_CORE_INFO("Imgui theme is set to Grey");

        ImVec4* colors = ImGui::GetStyle().Colors;
        // Text
        colors[ImGuiCol_Text]                   = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);

        colors[ImGuiCol_WindowBg]               = ImVec4(0.18f, 0.18f, 0.18f, 1.0f); // Window background
        
        colors[ImGuiCol_ChildBg]                = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
        
        colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        
        // Border
        colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.5f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        
        // Frame
        colors[ImGuiCol_FrameBg]                = ImVec4(0.3f, 0.3f, 0.3f, 0.5f); // Widget backgrounds
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.4f, 0.4f, 0.4f, 0.4f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.4f, 0.4f, 0.4f, 0.6f);
        
        // Title
        colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.0f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.29f, 0.29f, 0.29f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.0f, 0.0f, 0.0f, 0.51f);
        
        // Menu
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);
        
        // Scroll
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);
        
        // Check
        colors[ImGuiCol_CheckMark]              = ImVec4(0.94f, 0.94f, 0.94f, 1.0f);
        
        // SLider
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);
        
        // Button
        colors[ImGuiCol_Button]                 = ImVec4(0.44f, 0.44f, 0.44f, 0.4f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.46f, 0.47f, 0.48f, 1.0f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.42f, 0.42f, 0.42f, 1.0f);
        
        // Headers
        colors[ImGuiCol_Header]                 = ImVec4(0.7f, 0.7f, 0.7f, 0.31f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.7f, 0.7f, 0.7f, 0.8f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.48f, 0.5f, 0.52f, 1.0f);
        
        // Separator
        colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.5f, 0.5f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);
        
        // Resize
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
        
        // Plot
        colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.0f, 0.43f, 0.35f, 1.0f);
        colors[ImGuiCol_PlotHistogram]          = ImVec4(0.73f, 0.6f, 0.15f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
                
        colors[ImGuiCol_ModalWindowDarkening]   = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
        
        colors[ImGuiCol_DragDropTarget]         = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
        
        colors[ImGuiCol_NavHighlight]           = ImVec4(0.60f, 0.6f, 0.6f, 1.0f);
        colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
    }
    
    // ******************************************************************************
    // Dark grey theme
    // ******************************************************************************
    void ImGuiAPI::SetDarkThemeColors()
    {
        IK_CORE_INFO("Imgui theme is set to Dark");

        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg]               = ImVec4{ 0.05f, 0.05f, 0.05f, 1.0f };
                
        colors[ImGuiCol_ChildBg]                = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
        
        colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        
        // Text
        colors[ImGuiCol_Text]                   = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);

        // Headers
        colors[ImGuiCol_Header]                 = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };
        colors[ImGuiCol_HeaderHovered]          = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
        colors[ImGuiCol_HeaderActive]           = ImVec4{ 0.2f, 0.2, 0.2f, 1.0f };
        
        // Buttons
        colors[ImGuiCol_Button]                 = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered]          = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive]           = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        
        // Frame BG
        colors[ImGuiCol_FrameBg]                = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered]         = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive]          = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        
        // Tabs
        colors[ImGuiCol_Tab]                    = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered]             = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive]              = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused]           = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive]     = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        
        // Title
        colors[ImGuiCol_TitleBg]                = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive]          = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        
        // Border
        colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.5f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        
        // Menu
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.09f, 0.09f, 0.09f, 1.0f);

        // Check
        colors[ImGuiCol_CheckMark]              = ImVec4(0.94f, 0.94f, 0.94f, 1.0f);
        
        // SLider
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);
        
        // Separator
        colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.5f, 0.5f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);
        
        // SLider
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);
        
        // Resize
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
        
        // Plot
        colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.0f, 0.43f, 0.35f, 1.0f);
        colors[ImGuiCol_PlotHistogram]          = ImVec4(0.73f, 0.6f, 0.15f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
        
        colors[ImGuiCol_ModalWindowDarkening]   = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
        
        colors[ImGuiCol_DragDropTarget]         = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
        
        colors[ImGuiCol_NavHighlight]           = ImVec4(0.60f, 0.6f, 0.6f, 1.0f);
        colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
    }
    
    // ******************************************************************************
    // Light theme
    // ******************************************************************************
    void ImGuiAPI::SetLightThemeColors()
    {
        IK_CORE_INFO("Imgui theme is set to Light");

        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg]           = ImVec4{ 0.7f, 0.7f, 0.7f, 1.0f };
        
        // Headers
        colors[ImGuiCol_Header]             = ImVec4{ 0.7f, 0.705f, 0.71f, 1.0f };
        colors[ImGuiCol_HeaderHovered]      = ImVec4{ 0.6f, 0.605f, 0.61f, 1.0f };
        colors[ImGuiCol_HeaderActive]       = ImVec4{ 0.85f, 0.8505f, 0.851f, 1.0f };
        
        // Buttons
        colors[ImGuiCol_Button]             = ImVec4{ 0.7f, 0.705f, 0.71f, 1.0f };
        colors[ImGuiCol_ButtonHovered]      = ImVec4{ 0.6f, 0.605f, 0.61f, 1.0f };
        colors[ImGuiCol_ButtonActive]       = ImVec4{ 0.85f, 0.8505f, 0.851f, 1.0f };
        
        // Frame BG
        colors[ImGuiCol_FrameBg]            = ImVec4{ 0.7f, 0.705f, 0.71f, 1.0f };
        colors[ImGuiCol_FrameBgHovered]     = ImVec4{ 0.6f, 0.605f, 0.61f, 1.0f };
        colors[ImGuiCol_FrameBgActive]      = ImVec4{ 0.85f, 0.8505f, 0.851f, 1.0f };
        
        // Tabs
        colors[ImGuiCol_Tab]                = ImVec4{ 0.85f, 0.8505f, 0.851f, 1.0f };
        colors[ImGuiCol_TabHovered]         = ImVec4{ 0.68f, 0.6805f, 0.681f, 1.0f };
        colors[ImGuiCol_TabActive]          = ImVec4{ 0.78f, 0.7805f, 0.781f, 1.0f };
        colors[ImGuiCol_TabUnfocused]       = ImVec4{ 0.85f, 0.8505f, 0.851f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.7f, 0.705f, 0.71f, 1.0f };
        
        // Title
        colors[ImGuiCol_TitleBg]            = ImVec4{ 0.85f, 0.8505f, 0.851f, 1.0f };
        colors[ImGuiCol_TitleBgActive]      = ImVec4{ 0.85f, 0.8505f, 0.851f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed]   = ImVec4{ 0.85f, 0.8505f, 0.851f, 1.0f };
        
        // Texts
        colors[ImGuiCol_Text]                   = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
        
        // Scroll
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.92f, 0.92f, 0.92f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.71f, 0.71f, 0.71f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.61f, 0.61f, 0.61f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);
        
        // SLider
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.46f, 0.46f, 0.46f, 1.0f);
        
        // Menu
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.94f, 0.94f, 0.94f, 1.0f);
        
        // Frame
        colors[ImGuiCol_FrameBg]                = ImVec4(0.7f, 0.7f, 0.7f, 0.5f); // Widget backgrounds
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.6f, 0.6f, 0.6f, 0.4f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.6f, 0.6f, 0.6f, 0.6f);
        
        // Check
        colors[ImGuiCol_CheckMark]              = ImVec4(0.04f, 0.04f, 0.04f, 1.0f);
        
    }
}
