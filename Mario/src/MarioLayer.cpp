#include "MarioLayer.h"
#include "BackgroundTiles.h"
#include "NativScripts/CameraController.h"
#include "NativScripts/PlayerController.h"

namespace Mario {
        
    MarioLayer::MarioLayer()
    : Layer("Mario")
    {
    }

    MarioLayer::~MarioLayer()
    {
    }

    void MarioLayer::OnAttach()
    {
        ImGuiAPI::SetDarkThemeColors();
        
        /*
         Upload the Shader
         NOTE: Upload the shader before calling Renderer2D::Begin Scene
         */
        Renderer2D::SetShaader("../../Mario/assets/shaders/Shader.glsl");
 
        // Adding Scene
        m_ActiveScene = Ref<Scene>::Create();
        
        // Frame Buffers
        FramebufferSpecification fbSpec;
        fbSpec.Width  = s_WindowWidth;
        fbSpec.Height = s_WindowHeight;
        
        m_FrameBuffer = Framebuffer::Create(fbSpec);

        m_SceneHierarchyPannel.SetContext(m_ActiveScene);

        BackgroundTile::Init(m_ActiveScene);
        
        // Camera Entity
        {
            Ref<Scene> scene = m_ActiveScene;
            
            Entity cameraEntity = scene->CreateEntity("Camera");
            auto& camera = cameraEntity.AddComponent<CameraComponent>().Camera;
            camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
            camera.SetOrthographicSize(20.0);

            cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
        }
        
        // Player Entity
        {
            auto playerEntity = m_ActiveScene->CreateEntity("Player");
            playerEntity.AddComponent<SpriteRendererComponent>();
            
            playerEntity.AddComponent<NativeScriptComponent>().Bind<PlayerController>();
        }
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
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        RendererStatistics::Reset();
        m_FrameBuffer->Bind();
        
        Renderer::Clear(BackgroundTile::s_BgColor);
                    
        m_ActiveScene->OnUpdate(timeStep);
        
        m_FrameBuffer->Unbind();
    }
                    
    void MarioLayer::OnImguiRender()
    {
        static bool isFrameRate           = true;
        static bool isRendererStats       = true;
        static bool isVendorType          = true;
        static bool isSetting             = true;
        static bool isSceneHeirarchypanel = false;
        
        ImGuiAPI::EnableDcocking();
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
                    isSceneHeirarchypanel = true;
                if (ImGui::MenuItem("Setting"))
                    isSetting = true;
                if (ImGui::MenuItem("Frame Rate"))
                    isFrameRate = true;
                if (ImGui::MenuItem("Render Stats"))
                    isRendererStats = true;
                if (ImGui::MenuItem("Vendor Types"))
                    isVendorType = true;
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
            
            if (ImGui::CollapsingHeader("BackGround"))
                BackgroundTile::ImGuiRenderer();

            ImGui::End();
        }
        
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
