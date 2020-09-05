#include "MarioLayer.h"
#include "SceneMap.h"
namespace iKan {
    
    static glm::vec4 s_BgColor = { 0.3f, 0.1f, 0.6f, 1.0f };
    static float s_Speed = 5.0f;
    static std::string GetEntityNameFromChar(char type)
    {
        switch(type)
        {
            case 'G' : return "Ground";                 break;
            case '|' : return "Castel Brick";           break;
            case 'o' : return "Castel Gate";            break;
            case 'u' : return "castel Gate Domb";       break;
            case '.' : return "Castel Domb";            break;
            case 'l' : return "Castel Windlow Left";    break;
            case 'r' : return "Castel Window Right";    break;
            case 'S' : return "Steps";                  break;
            case '-' : return "Bridge";                 break;
            case '!' : return "Pipe Base";              break;
            case 'Y' : return "Pipe Head";              break;
            case 'X' : return "Bricks";                 break;
            case 'B' : return "Bonus";                  break;
        }
        IK_ASSERT(false, "Invalid Type");
        return "";
    }
   
    MarioLayer::MarioLayer()
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
 
        // Frame Buffers
        FramebufferSpecification fbSpec;
        fbSpec.Width  = s_WindowWidth;
        fbSpec.Height = s_WindowHeight;
        
        m_FrameBuffer = Framebuffer::Create(fbSpec);
        
        m_PlayerInstance = Player::Create();
        
        // Texture tile
        m_PlayerSpriteSheet = Texture::Create("../../Mario/assets/Resources/Graphics/Player.png");
        m_TileSpriteSheet   = Texture::Create("../../Mario/assets/Resources/Graphics/Tile.png");
        m_MapWidth          = s_MapWidth;
        m_MapHeight         = static_cast<uint32_t>(strlen(s_MapTiles)) / s_MapWidth;

        // Adding Texture maps
        // Ground
        m_TextureMap['G'] = SubTexture::CreateFromCoords(m_TileSpriteSheet, { 0, 21 });
        
        // Castel
        m_TextureMap['.'] = SubTexture::CreateFromCoords(m_TileSpriteSheet, { 19, 25 });
        m_TextureMap['u'] = SubTexture::CreateFromCoords(m_TileSpriteSheet, { 20, 24 });
        m_TextureMap['o'] = SubTexture::CreateFromCoords(m_TileSpriteSheet, { 21, 24 });
        m_TextureMap['|'] = SubTexture::CreateFromCoords(m_TileSpriteSheet, { 21, 25 });
        m_TextureMap['l'] = SubTexture::CreateFromCoords(m_TileSpriteSheet, { 20, 25 });
        m_TextureMap['r'] = SubTexture::CreateFromCoords(m_TileSpriteSheet, { 22, 25 });
        
        // Steps
        m_TextureMap['S'] = SubTexture::CreateFromCoords(m_TileSpriteSheet, { 0, 22 });
        
        // Bridge
        m_TextureMap['-'] = SubTexture::CreateFromCoords(m_TileSpriteSheet, { 15, 19 });
        
        // Pipes
        m_TextureMap['!'] = SubTexture::CreateFromCoords(m_TileSpriteSheet, { 0, 8 }, { 2, 1 });
        m_TextureMap['Y'] = SubTexture::CreateFromCoords(m_TileSpriteSheet, { 0, 9 }, { 2, 1 });
        
        // Bonus and Bricks
        m_TextureMap['X'] = SubTexture::CreateFromCoords(m_TileSpriteSheet, { 17, 25 });;
        m_TextureMap['B'] = SubTexture::CreateFromCoords(m_TileSpriteSheet, { 24,  21 });
        
        // Player Subtexture
        m_PlayerSubtexture = SubTexture::CreateFromCoords(m_PlayerSpriteSheet, { 0, 12 });

        // Adding Scene
        m_Scene = std::make_shared<Scene>();
        
        for (uint32_t y = 0; y < m_MapHeight; y++)
        {
            for (uint32_t x = 0; x < m_MapWidth; x++)
            {
                char tileType = s_MapTiles[x + y * m_MapWidth];
                if (m_TextureMap.find(tileType) != m_TextureMap.end())
                {
                    std::shared_ptr<SubTexture> subTexture = m_TextureMap[tileType];
                    
                    Entity entity     = m_Scene->CreateEntity(GetEntityNameFromChar(tileType));
                    auto spriteEntity = entity.AddComponent<SpriteRendererComponent>(subTexture);
                    auto spriteSize   = spriteEntity.SubTexComp->GetSpriteSize();
                    
                    entity.GetComponent<TransformComponent>().SetTransform({ x, (m_MapHeight / 2.0f) - y }, { spriteSize.x, spriteSize.y });
                }
            }
        }
        
        m_PlayerEntity = m_Scene->CreateEntity("Player");
        m_PlayerEntity.AddComponent<SpriteRendererComponent>(m_PlayerSubtexture);

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
        RenderCommand::SetClearColor(s_BgColor);
                
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
        if (ImGui::CollapsingHeader("BackGround"))
        {
            if (ImGui::TreeNode("Color"))
            {
                static ImVec4 color         = ImVec4(s_BgColor.r, s_BgColor.g, s_BgColor.b, s_BgColor.a);
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
                ImGui::TreePop();
                
                s_BgColor = { color.x, color.y, color.z, color.w };
            }
        }
        
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
