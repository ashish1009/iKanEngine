// ******************************************************************************
//   File    : MarioLayer.cpp
//   Project : Mario
//
//   Created by Ashish
// ******************************************************************************

#include "MarioLayer.h"

namespace Mario {
        
    // ******************************************************************************
    // Mario Layer Constructor
    // ******************************************************************************
    MarioLayer::MarioLayer()
    : Layer("Mario Layer")
    {
        IK_INFO("Mario Layer Constructor called");
    }

    // ******************************************************************************
    // Mario Layer Destructor
    // ******************************************************************************
    MarioLayer::~MarioLayer()
    {
        IK_WARN("Mario Layer Destructor called");
    }

    // ******************************************************************************
    // Mario Layer On attach
    // ******************************************************************************
    void MarioLayer::OnAttach()
    {
        IK_INFO("Mario Layer is attached to the appliaction");
        
       ImGuiAPI::SetDarkThemeColors();
        
        // Upload the Shader
        // NOTE: Upload the shader before calling Renderer2D::Begin Scene
        // As in begin scene Shader need to be bind for texture
        Renderer2D::SetShaader("../../Mario/assets/shaders/Shader.glsl");
 
        // Creating Scene
        m_ActiveScene = Ref<Scene>::Create();
        
        // Frame Buffers specifications
        FramebufferSpecification fbSpec;
        fbSpec.Attachments = { FbTextureFormat::RGBA8 };
        fbSpec.Width  = s_WindowWidth;
        fbSpec.Height = s_WindowHeight;
        
        m_Viewport.FrameBuffer = Framebuffer::Create(fbSpec);

        m_SceneHierarchyPannel.SetContext(m_ActiveScene);

        // TODO: Find better place for this,may be to move in player class
        {
            // Main Camera Entity is created
            IK_INFO("Creating Main camera entity for Mario");
            Ref<Scene> scene    = m_ActiveScene;
            Entity cameraEntity = scene->CreateEntity("Camera");
            auto& camera        = cameraEntity.AddComponent<CameraComponent>().Camera;
            
            camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
            camera.SetOrthographicSize(20.0);

            cameraEntity.GetComponent<TransformComponent>().Translation.x = 0.0f;;
        }
    }
    
    // ******************************************************************************
    // Mario Layer On detach
    // ******************************************************************************
    void MarioLayer::OnDetach()
    {
        IK_WARN("Mario Layer is detached to the appliaction");
    }
    
    // ******************************************************************************
    // Mario Layer On Event
    // ******************************************************************************
    void MarioLayer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(IK_BIND_EVENT_FN(MarioLayer::OnKeyPressed));
        dispatcher.Dispatch<KeyReleasedEvent>(IK_BIND_EVENT_FN(MarioLayer::OnKeyReleased));
    }
    
    // ******************************************************************************
    // Mario Layer Key Pressed Event handler
    // ******************************************************************************
    bool MarioLayer::OnKeyPressed(KeyPressedEvent& event)
    {
        return false;
    }
    
    // ******************************************************************************
    // Mario Layer Key Relesed Event Handler
    // ******************************************************************************
    bool MarioLayer::OnKeyReleased(KeyReleasedEvent& event)
    {
        return false;
    }
                                 
    // ******************************************************************************
    // Mario Layer Updaete each frame
    // ******************************************************************************
    void MarioLayer::OnUpdate(TimeStep timeStep)
    {
        // If resize the window call the update the Scene View port and Frame buffer should be resized
        if (FramebufferSpecification spec = m_Viewport.FrameBuffer->GetSpecification();
            m_Viewport.Size.x > 0.0f && m_Viewport.Size.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_Viewport.Size.x || spec.Height != m_Viewport.Size.y))
        {
            m_Viewport.FrameBuffer->Resize((uint32_t)m_Viewport.Size.x, (uint32_t)m_Viewport.Size.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_Viewport.Size.x, (uint32_t)m_Viewport.Size.y);
        }

        RendererStatistics::Reset();
        
        m_Viewport.FrameBuffer->Bind();
        {
            Renderer::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });
            m_ActiveScene->OnUpdateRuntime(timeStep);
        }
        m_Viewport.FrameBuffer->Unbind();
    }
                    
    // ******************************************************************************
    // Mario Layer Render Imgui each frame
    // ******************************************************************************
    void MarioLayer::OnImguiRender()
    {
        static bool isFrameRate           = true;
        static bool isRendererStats       = true;
        static bool isVendorType          = true;
        static bool isSetting             = false;
        static bool isSceneHeirarchypanel = false;

        // Imgui docking
        ImGuiAPI::EnableDcocking();
        {
            // ----------------------- Menu Bar ---------------------------------------------------------------
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Exit")) Application::Get().Close();
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("View"))
                {
                    if (ImGui::MenuItem("Scene Heirarchy Panel"))
                        isSceneHeirarchypanel = !isSceneHeirarchypanel;
                    if (ImGui::MenuItem("Setting"))
                        isSetting = !isSetting;
                    if (ImGui::MenuItem("Frame Rate"))
                        isFrameRate = !isFrameRate;
                    if (ImGui::MenuItem("Render Stats"))
                        isRendererStats = !isRendererStats;
                    if (ImGui::MenuItem("Vendor Types"))
                        isVendorType = !isVendorType;
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Properties"))
                {
                    if (ImGui::BeginMenu("Theme"))
                    {
                        if (ImGui::MenuItem("Light"))
                            ImGuiAPI::SetLightThemeColors();
                        if (ImGui::MenuItem("Dark"))
                            ImGuiAPI::SetDarkThemeColors();
                        if (ImGui::MenuItem("Grey"))
                            ImGuiAPI::SetGreyThemeColors();
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            
            if (isFrameRate)
                ImGuiAPI::FrameRate(&isFrameRate);

            if (isRendererStats)
                ImGuiAPI::RendererStats(&isRendererStats);
            
            if (isVendorType)
                ImGuiAPI::RendererVersion(&isVendorType);
            
            if (isSceneHeirarchypanel)
                m_SceneHierarchyPannel.OnImguiender(&isSceneHeirarchypanel);

            // ----------------------- Setings ----------------------------------------------------------------
            if (isSetting)
            {
                ImGui::Begin("Setting", &isSetting);
                ImGui::End();
            }
            
            //------------------------ View Port ---------------------------------------------------------------
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
            ImGui::Begin("Viewport");
            
            m_Viewport.Focused = ImGui::IsWindowFocused();
            m_Viewport.Hovered = ImGui::IsWindowHovered();
            Application::Get().GetImGuiLayer()->BlockEvents(!m_Viewport.Focused || !m_Viewport.Hovered);
            
            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            m_Viewport.Size = { viewportPanelSize.x, viewportPanelSize.y };
            
            size_t textureID = m_Viewport.FrameBuffer->GetColorAttachmentRendererID();
            ImGui::Image((void*)textureID, ImVec2{ m_Viewport.Size.x, m_Viewport.Size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
            ImGui::End();
            ImGui::PopStyleVar();
        }
        // Ending of Docking egining
        ImGuiAPI::EndDocking();

    }
                                    
}
