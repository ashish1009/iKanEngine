// ******************************************************************************
//   File    : MarioLayer.cpp
//   Project : Mario
//
//   Created by Ashish
// ******************************************************************************

#include "MarioLayer.h"
#include "Background.h"

namespace Mario {

    MarioLayer::PropertyFlag MarioLayer::s_PropFlag;
        
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
        
        IK_INFO("Adding Mario Shader to Renderer");
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

            cameraEntity.GetComponent<TransformComponent>().Translation.x = 19.0f;
        }
        
        BackgroundTile::CreateEntities(m_ActiveScene);
        m_Player.OnInit(m_ActiveScene);
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
        
        m_Player.OnUpdate(timeStep);
        
        m_Viewport.FrameBuffer->Bind();
        {
            Renderer::Clear(BackgroundTile::s_BgColor);
            m_ActiveScene->OnUpdateRuntime(timeStep);
        }
        m_Viewport.FrameBuffer->Unbind();
    }

    // ******************************************************************************
    // Mario Layer Render Imgui each frame
    // ******************************************************************************
    void MarioLayer::OnImguiRender()
    {
        // Imgui docking
        ImGuiAPI::EnableDcocking();
        {
            MarioLayer::ShowMenue();
            MarioLayer::RendererStats();
            
            // Show Scene hierarchy pannel in imgui
            if (MarioLayer::s_PropFlag.IsSceneHeirarchypanel)
            {
                m_SceneHierarchyPannel.OnImguiender(&MarioLayer::s_PropFlag.IsSceneHeirarchypanel);
            }

            // Show mario Setting in Imgui
            if (MarioLayer::s_PropFlag.IsSetting)
            {
                ImGui::Begin("Setting", &MarioLayer::s_PropFlag.IsSetting);
                BackgroundTile::ImGuiRenderer();
                ImGui::End();
            }
            
            // View Port update
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
            ImGui::Begin("Viewport");
            {
                m_Viewport.OnUpdate();
            }
            ImGui::End();
            ImGui::PopStyleVar();
        }
        // Ending of Docking egining
        ImGuiAPI::EndDocking();
    }
    
    // ******************************************************************************
    // Show menu for Mario
    // ******************************************************************************
    void MarioLayer::ShowMenue()
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
            }
            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Scene Heirarchy Panel"))
                {
                    MarioLayer::s_PropFlag.IsSceneHeirarchypanel = !MarioLayer::s_PropFlag.IsSceneHeirarchypanel;
                }
                if (ImGui::MenuItem("Setting"))
                {
                    MarioLayer::s_PropFlag.IsSetting = !MarioLayer::s_PropFlag.IsSetting;
                }
                if (ImGui::MenuItem("Frame Rate"))
                {
                    MarioLayer::s_PropFlag.IsFrameRate = !MarioLayer::s_PropFlag.IsFrameRate;
                }
                if (ImGui::MenuItem("Render Stats"))
                {
                    MarioLayer::s_PropFlag.IsRendererStats = !MarioLayer::s_PropFlag.IsRendererStats;
                }
                if (ImGui::MenuItem("Vendor Types"))
                {
                    MarioLayer::s_PropFlag.IsVendorType = !MarioLayer::s_PropFlag.IsVendorType;
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Properties"))
            {
                if (ImGui::BeginMenu("Theme"))
                {
                    if (ImGui::MenuItem("Light"))
                    {
                        ImGuiAPI::SetLightThemeColors();
                    }
                    if (ImGui::MenuItem("Dark"))
                    {
                        ImGuiAPI::SetDarkThemeColors();
                    }
                    if (ImGui::MenuItem("Grey"))
                    {
                        ImGuiAPI::SetGreyThemeColors();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }
    
    // ******************************************************************************
    // Show the renderer stats
    // ******************************************************************************
    void MarioLayer::RendererStats()
    {
        if (MarioLayer::s_PropFlag.IsFrameRate)
        {
            ImGuiAPI::FrameRate(&MarioLayer::s_PropFlag.IsFrameRate);
        }

        if (MarioLayer::s_PropFlag.IsRendererStats)
        {
            ImGuiAPI::RendererStats(&MarioLayer::s_PropFlag.IsRendererStats);
        }
        
        if (MarioLayer::s_PropFlag.IsVendorType)
        {
            ImGuiAPI::RendererVersion(&MarioLayer::s_PropFlag.IsVendorType);
        }
    }
                                    
}
