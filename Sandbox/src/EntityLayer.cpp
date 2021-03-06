#include "EntityLayer.h"

namespace iKan {
    
#define IsCollision(x) (m_Entity.GetScene()->CollisionDetection(m_Entity, speed * ts) & (int)Scene::CollisionSide::x)
    
    class BoxController : public ScriptableEntity
    {
    public:
        void OnCreate()
        {
        }
        
        void OnUpdate(TimeStep ts)
        {
            if (HasComponent<TransformComponent>())
            {
                auto& translation = GetComponent<TransformComponent>().Translation;
                float speed = 2.5f;
                
                if(Input::IsKeyPressed(KeyCode::Left))
                {
                    if (!IsCollision(Left))
                    {
                        translation.x -= speed * ts;
                    }
                }
                if(Input::IsKeyPressed(KeyCode::Right))
                {
                    if (!IsCollision(Right))
                    {
                        translation.x += speed * ts;
                    }
                }
                if(Input::IsKeyPressed(KeyCode::Down))
                    translation.y += speed * ts;
                if(Input::IsKeyPressed(KeyCode::Up))
                    translation.y -= speed * ts;
            }
        }
        
        void OnDestroy()
        {
        }
        
        // OnAwake(), CollisionCallbacks()
    };
    
    EntityLayer::EntityLayer()
    {
        
    }
    
    EntityLayer::~EntityLayer()
    {
        
    }
    
    void EntityLayer::OnAttach()
    {
        ImGuiAPI::SetDarkThemeColors();
        
        // TODO: Set the position of Shader later
        Renderer2D::SetShaader("../../Mario/assets/shaders/Shader.glsl");
        
        FramebufferSpecification fbSpec;
        fbSpec.Width  = 1280;
        fbSpec.Height = 720;
        
        m_Framebuffer = Framebuffer::Create(fbSpec);
        m_ActiveScene = Ref<Scene>::Create();
        
        auto blueSquare = m_ActiveScene->CreateEntity("Green Square");
        blueSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.1f, 0.7f, 0.1f, 1.0f });
        blueSquare.AddComponent<BoxCollider2DComponent>();
        blueSquare.GetComponent<TransformComponent>().Translation.x = 2;
        
        auto redSquare = m_ActiveScene->CreateEntity("Red Square");
        redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.7f, 0.1f, 0.1f, 1.0f });
        redSquare.AddComponent<BoxCollider2DComponent>();
        redSquare.GetComponent<TransformComponent>().Translation.x = -2;

        redSquare.AddComponent<NativeScriptComponent>().Bind<BoxController>();
        
        auto cameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
        cameraEntity.AddComponent<CameraComponent>().Camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
        
        class CameraController : public ScriptableEntity
        {
        public:
            void OnCreate()
            {
            }
            
            void OnUpdate(TimeStep ts)
            {
                if (HasComponent<TransformComponent>())
                {
                    auto& translation = GetComponent<TransformComponent>().Translation;
                    float speed = 0.5f;
                    
                    if(Input::IsKeyPressed(KeyCode::D))
                        translation.x -= speed * ts;
                    if(Input::IsKeyPressed(KeyCode::A))
                        translation.x += speed * ts;
                    if(Input::IsKeyPressed(KeyCode::S))
                        translation.y += speed * ts;
                    if(Input::IsKeyPressed(KeyCode::W))
                        translation.y -= speed * ts;
                }
            }
            
            void OnDestroy()
            {
            }
            
            // OnAwake(), CollisionCallbacks()
        };
        cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
    }
    
    void EntityLayer::OnDetach()
    {
        
    }
    
    void EntityLayer::OnEvent(Event& event)
    {
        
    }
    
    void EntityLayer::OnUpdate(TimeStep timeStep)
    {
        /*
         This will render the 'old' sized framebuffer onto the 'new' sized ImGuiPanel
         and store the 'new' size in m_ViewportSize.
         The next frame will first resize the framebuffer as m_ViewportSize differs
         from m_Framebuffer.Width/Height before updating and rendering.
         This results in never rendering an empty (black) framebuffer.
         */
        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }
        
        RendererStatistics::Reset();
        
        m_Framebuffer->Bind();
        
        Renderer::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });
        
        m_ActiveScene->OnUpdate(timeStep);
        
        m_Framebuffer->Unbind();
        
        m_SceneHierarchyPannel.SetContext(m_ActiveScene);

    }
    
    void EntityLayer::OnImguiRender()
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
        
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        style.WindowMinSize.x = minWinSizeX;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                {
                    Application::Get().Close();
                }
                if (ImGui::MenuItem("Serialize"))
                {
                    SceneSerializer serializer(m_ActiveScene);
                    serializer.Serialize("../../Sandbox/assets/Scene/Example.iKan");
                }
                if (ImGui::MenuItem("Deserialize"))
                {
                    SceneSerializer serializer(m_ActiveScene);
                    serializer.Deserialize("../../Sandbox/assets/Scene/Example.iKan");
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        
        ImGuiAPI::FrameRate();
        ImGuiAPI::RendererStats();
        ImGuiAPI::RendererVersion();
        
        m_SceneHierarchyPannel.OnImguiender();
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");
        
        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
        
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        
        size_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
        ImGui::PopStyleVar();
        
        ImGui::End();
    }
    
}
