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
    }
    
    void MarioLayer::OnDetach()
    {
    }
    
    void MarioLayer::OnEvent(Event& event)
    {
    }
                             
    void MarioLayer::OnUpdate(TimeStep timeStep)
    {
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
        
        ImGui::End();
        ImGui::PopStyleVar();
        
        ImGui::End();
    }
                                   
}
