#include <iKan/ImGui/ImGuiAPI.h>
#include <iKan/Renderer/Renderer2D.h>
#include <iKan/Renderer/RenderAPI.h>

namespace iKan {
    
    void ImGuiAPI::RendererVersion()
    {
        ImGui::Begin("Renderer");
        auto& caps = RenderAPI::GetCapabilities();

        ImGui::Text("Vendor : %s", caps.Vendor.c_str());
        ImGui::Text("Renderer : %s", caps.Renderer.c_str());
        ImGui::Text("Version : %s", caps.Version.c_str());

        ImGui::End();
    }
    
    void ImGuiAPI::StatsAndFrameRate()
    {
        //------------------------ Statistics -------------------------------------------------------------
        ImGui::Begin("Stats");
        const auto& stats = Renderer2D::GetStats();
        ImGui::Text("Draw Calls : %d", stats.DrawCalls);
        ImGui::Text("Quad Counts : %d", stats.QuadCount);
        ImGui::Text("VertexCounts : %d", stats.GetTotalVertexCount());
        ImGui::Text("Index Counts : %d", stats.GetTotalIndexCount());
        ImGui::End();
        
        //------------------------ Frame Rates -------------------------------------------------------------
        ImGui::Begin("Frame Rate");
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
    
    void ImGuiAPI::Counter(const std::string& name, uint32_t& counter)
    {
#pragma clang diagnostic ignored "-Wformat-security"
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

    void ImGuiAPI::Counter(const std::string& name, float& value)
    {
        uint32_t counter = (uint32_t)value;
        Counter(name, counter);
        value = (uint32_t)counter;
    }
    
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
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
    }
    
    void ImGuiAPI::EndDocking()
    {
        ImGui::End();
    }
}
