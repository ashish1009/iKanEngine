// ******************************************************************************
//   File    : EditorLayer.cpp
//   Project : iKanEditor
//
//   Created by Ashish
// ******************************************************************************

#include "EditorLayer.h"

static const std::string s_LayerName = "iKan Editor Layer";

// ******************************************************************************
// Editor Constructor
// ******************************************************************************
Editor::Editor()
: Layer(s_LayerName.c_str()), m_EditorCamera(glm::radians(45.0f), 1800.0f/800.0f, 0.01f, 10000.0f)
{
    IK_INFO("iKan Editor Layer constructed");
}

// ******************************************************************************
// Editor Destructor
// ******************************************************************************
Editor::~Editor()
{
    IK_WARN("iKan Editor Layer destroyed");
}

// ******************************************************************************
// Attach Editor layer
// ******************************************************************************
void Editor::OnAttach()
{
    IK_INFO("iKan Editor Layer attached");

    ImGuiAPI::SetGreyThemeColors();

    // Specification for framebuffers
    FramebufferSpecification specs;
    specs.Attachments = { FbTextureFormat::RGBA8, FbTextureFormat::Depth, FbTextureFormat::R32I};
    specs.Width       = s_WindowWidth;
    specs.Height      = s_WindowWidth;

    // Viewport framebuffer
    m_Viewport.FrameBuffer = Framebuffer::Create(specs);

}

// ******************************************************************************
// Detach Editor layer
// ******************************************************************************
void Editor::OnDetach()
{
    IK_WARN("iKan Editor Layer detached");
}

// ******************************************************************************
// Event update for edtor layer
// ******************************************************************************
void Editor::OnEvent(Event& event)
{
    m_EditorCamera.OnEvent(event);
}

// ******************************************************************************
// Update Editor layer per frame
// ******************************************************************************
void Editor::OnUpdate(TimeStep timeStep)
{
    // If resize the window call the update the Scene View port and Frame buffer should be resized
    if (FramebufferSpecification spec = m_Viewport.FrameBuffer->GetSpecification();
        m_Viewport.Size.x > 0.0f && m_Viewport.Size.y > 0.0f && // zero sized framebuffer is invalid
        (spec.Width != m_Viewport.Size.x || spec.Height != m_Viewport.Size.y))
    {
        m_Viewport.FrameBuffer->Resize((uint32_t)m_Viewport.Size.x, (uint32_t)m_Viewport.Size.y);
        m_EditorCamera.SetViewportSize((uint32_t)m_Viewport.Size.x, (uint32_t)m_Viewport.Size.y);
    }

    // Update the camera per frame
    m_EditorCamera.OnUpdate(timeStep);

}

// ******************************************************************************
// ImGui renderer per frame
// ******************************************************************************
void Editor::OnImguiRender()
{
    ImGuiAPI::EnableDcocking();
    {
        // Menu handling
        OpenMenue();

        // Stats and Version
        ImGuiAPI::FrameRate();
        ImGuiAPI::RendererStats();
        ImGuiAPI::RendererVersion();

        // View Port
        OnViewport();

    } // ImGuiAPI::EnableDcocking();
    ImGuiAPI::EndDocking();
}

// ******************************************************************************
// Maneg the view port
// ******************************************************************************
void Editor::OnViewport()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin(s_LayerName.c_str());

    m_Viewport.Focused = ImGui::IsWindowFocused();
    m_Viewport.Hovered = ImGui::IsWindowHovered();
    Application::Get().GetImGuiLayer()->BlockEvents(!m_Viewport.Focused && !m_Viewport.Hovered);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_Viewport.Size = { viewportPanelSize.x, viewportPanelSize.y };

    size_t textureID = m_Viewport.FrameBuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)textureID, ImVec2{ m_Viewport.Size.x, m_Viewport.Size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    ImGui::End(); // For View port begin
    ImGui::PopStyleVar(); // For View port push

}

// ******************************************************************************
// Handles thhe menu of Editor
// ******************************************************************************
void Editor::OpenMenue()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                Application::Get().Close();
            }
            ImGui::EndMenu();
        } // if (ImGui::BeginMenu("File"))

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
            } // if (ImGui::BeginMenu("Theme"))
            ImGui::EndMenu();
        } // if (ImGui::BeginMenu("Properties"))
        ImGui::EndMenuBar();
    } // if (ImGui::BeginMenuBar())

}
