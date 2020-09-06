#include "MarioLayer.h"

#include "Layers/Background.h"

namespace iKan {
    
    static float s_Speed = 5.0f;
    
    MarioLayer::MarioLayer()
    : Layer("Mario")
    {
    }

    MarioLayer::~MarioLayer()
    {
        Player::Destroy();
    }

    void MarioLayer::OnAttach()
    {
        /*
         Upload the Shader
         NOTE: Upload the shader before calling Renderer2D::Begin Scene
         */
        Renderer2D::SetShaader("../../Mario/assets/shaders/Shader.glsl");
 
        // Adding Scene
        m_Scene = std::make_shared<Scene>();
        
        // Frame Buffers
        FramebufferSpecification fbSpec;
        fbSpec.Width  = s_WindowWidth;
        fbSpec.Height = s_WindowHeight;
        
        m_FrameBuffer = Framebuffer::Create(fbSpec);
        
        m_PlayerInstance = Player::Create(m_Scene);
        Background::Init(m_Scene);

        m_CameraEntity = m_Scene->CreateEntity("Camera");
        m_CameraEntity.AddComponent<CameraComponent>();
        
        // Temporary
        class CameraController : public ScriptableEntity
        {
        public:
            void OnCreate()
            {
            }
            
            void OnUpdate(TimeStep ts)
            {
                auto& transform = GetComponent<TransformComponent>().Transform;
                float speed = s_Speed;
                
                if(Input::IsKeyPressed(Key::Left))
                    transform[3][0] -= speed * ts;
                if(Input::IsKeyPressed(Key::Right))
                    transform[3][0] += speed * ts;
            }
            
            void OnDestroy()
            {
            }
        };
        
        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
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
        RenderCommand::SetClearColor(Background::GetColor());
                
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
        Background::ImGuiRender();
        
        //-------------------------- Camera Speed --------------------------------
        ImGui::Text("Camera Speed");
        ImGui::SameLine();
        
        // Arrow buttons with Repeater
        static int counter  = 0;
        float spacing       = ImGui::GetStyle().ItemInnerSpacing.x;
        
        ImGui::PushButtonRepeat(true);
        if (ImGui::ArrowButton("##left", ImGuiDir_Left))
            counter--;
        
        ImGui::SameLine(0.0f, spacing);
        if (ImGui::ArrowButton("##right", ImGuiDir_Right))
            counter++;
        
        ImGui::PopButtonRepeat();
        ImGui::SameLine();
        ImGui::Text("%d", counter);
        s_Speed = float(counter);
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
