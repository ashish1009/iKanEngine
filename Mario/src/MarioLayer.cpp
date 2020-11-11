#include "MarioLayer.h"

namespace iKan {
    
    static const uint32_t s_MapWidth = 212 * 2;
    /*
     G : Ground
     | : Castel Brick
     o : Castel Gate
     u : castel Gate Domb
     . : Castel Domb
     l : Castel Windlow Left
     r : Castel Window Right
     S : Steps
     - : Bridge
     ! : Pipe Base
     Y : Pipe Head
     X : Bricks
     B : Bonus
     */
    static const char* s_MapTiles =
    "                                                                                                                                                                                                                                                                                                                                                                                                                                        "
    "                                                                                                                                                                                                                                                                                                                                                                                                                                        "
    "                                                                                                                                                                                                                                                                                                                                                                                                                                        "
    "                                                                                                                                                                                                                                                                                                                                                                                                                               .        "
    "        XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX      XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX        XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX                  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX        XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX                         |        "
    "                                                                                                                                                                                                                                                                                                                                                                                                                              ...       "
    "                                                                                                                                                                                                                                                                                                                                                                                                                              |u|       "
    "                                                                                                                                                                                                                                                                                                                                                                                                                              |o|       "
    "                                                                                                                                                                                                                                                                                                                                                                                                                            .......     "
    "XXXX             B                  XXXX                             B            XBX                                                            B                                    B                               XXXXXXXX     XXXXX           B           XXXXX XX  XX XXXXX                                                                           B                                                               |u|r|u|     "
    "                                                                                                                                                                                                                                                                                                                                                                                                                            |o|||o|     "
    "                                                                                                                                                                                                                                                                                                                                                                                                     S                    ...........   "
    "                                                                                                                                                                                                                                                                                                                                                                                                    SS                    |||||||||||   "
    "                                                                                                                                                                                     SS                                                                                                                                                                                                            SSS                    |l|u|r|u|l|   "
    "               XBXBX              XBXXXXBX                           S           XXXXX               B                                           S                                  SSS                            XXXX      XXXX             B    B    B             XXXX                                                                                XBXBX                                   SSSS                    |||o|||o|||   "
    "                                                  Y                 SS                                                                          SS                                 SSSS                                                                                                                                           S  S                                                           SSSSS                  ............... "
    "                                                  !                SSS                                                   S  S                  SSS                                SSSSS                                                                                                                      Y                   SS  SS                                                         SSSSSS                  ||||||||||||||| "
    "                            Y                     !               SSSS                               Y                  SS  SS                SSSS             Y                 SSSSSS                                                                                                                      !                  SSS  SSS                    Y                                  SSSSSSS                  |u||u||u||u||u| "
    "                   S        !       S  S          !              SSSSS                               !                 SSS  SSS              SSSSS             !                SSSSSSS                                                                                                                      !                 SSSS  SSSS                   !                                 SSSSSSSS                  |o||o||o||o||0| "
    "GGGGGG GG GGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG--------------------GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGG GGGGGGGGGGGGGGG  GGGGGGGGGGGGG-----------------------------------------------------------------------------------------------GGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    "GGGGGG GG GGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG                    GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGG GGGGGGGGGGGGGGG  GGGGGGGGGGGGG                                                                                               GGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
    ;
    
    // To store the Tile sprite
    static Ref<Texture> s_TileSpriteSheet;
    
    // Map of Subtexture to the Char of their corresponding
    static std::unordered_map<char, Ref<SubTexture>> s_TextureMap;
    
    // Map of group of Entity to the Char of their corresponding
    static std::unordered_map<char, std::vector<Entity>> s_EntityVector;
    
    // Map of gorup of Subtextures to the Char of their corresponding
    static std::unordered_map<char, std::vector<Ref<SubTexture>>> s_SubTextureVectorMap;
    
    glm::vec4 s_BgColor = { 0.3f, 0.1f, 0.6f, 1.0f };

    static std::string GetEntityNameFromChar(char type)
    {
        switch(type)
        {
            case 'G' : return "Ground";              break;
            case '|' : return "Castel Brick";        break;
            case 'o' : return "Castel Gate";         break;
            case 'u' : return "castel Gate Domb";    break;
            case '.' : return "Castel Domb";         break;
            case 'l' : return "Castel Windlow Left"; break;
            case 'r' : return "Castel Window Right"; break;
            case 'S' : return "Steps";               break;
            case '-' : return "Bridge";              break;
            case '!' : return "Pipe Base";           break;
            case 'Y' : return "Pipe Head";           break;
            case 'X' : return "Bricks";              break;
            case 'B' : return "Bonus";               break;
        }
        IK_ASSERT(false, "Invalid Type");
        return "";
    }
        
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
        
        // Level 1 Map
        {
            Ref<Scene> scene = m_ActiveScene;
            // Texture tile
            s_TileSpriteSheet = Texture::Create("../../Mario/assets/Resources/Graphics/Tile.png");
            
            uint32_t mapWidth  = s_MapWidth;
            uint32_t mapHeight = static_cast<uint32_t>(strlen(s_MapTiles)) / s_MapWidth;
            
            // Ground SubTexutres
            {
                s_SubTextureVectorMap['G'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 27.0f })); // Brown
                s_SubTextureVectorMap['G'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 25.0f })); // Blue
                s_SubTextureVectorMap['G'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 23.0f })); // Grey
                s_SubTextureVectorMap['G'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 21.0f })); // Green
                
                s_TextureMap['G'] = s_SubTextureVectorMap['G'][3];
            }
            
            // Bricks SubTextures
            {
                s_SubTextureVectorMap['X'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 17.0f, 27.0f })); // Brown;
                s_SubTextureVectorMap['X'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 17.0f, 25.0f })); // Blue;
                s_SubTextureVectorMap['X'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 17.0f, 23.0f })); // Grey;
                s_SubTextureVectorMap['X'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 17.0f, 21.0f })); // green;
                
                s_TextureMap['X'] = s_SubTextureVectorMap['X'][1];
            }
            
            // Bomus SubTextures
            {
                s_SubTextureVectorMap['B'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 24.0f, 27.0f })); // Brown
                s_SubTextureVectorMap['B'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 24.0f, 25.0f })); // Blue
                s_SubTextureVectorMap['B'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 24.0f, 23.0f })); // Grey
                s_SubTextureVectorMap['B'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 24.0f, 21.0f })); // Green
                
                s_TextureMap['B'] = s_SubTextureVectorMap['B'][3];
            }
            
            // Steos SubTextures
            {
                s_SubTextureVectorMap['S'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 26.0f })); // Brown
                s_SubTextureVectorMap['S'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 24.0f })); // Blue
                s_SubTextureVectorMap['S'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 22.0f })); // Grey
                s_SubTextureVectorMap['S'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 20.0f })); // Green
                
                s_TextureMap['S'] = s_SubTextureVectorMap['S'][2];
            }
            
            // Bridge SubTexture
            {
                s_SubTextureVectorMap['-'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 15.0f, 19.0f })); // Green
                s_SubTextureVectorMap['-'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 15.0f, 15.0f })); // Orange
                s_SubTextureVectorMap['-'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 15.0f, 11.0f })); // Grey
                s_SubTextureVectorMap['-'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 15.0f, 9.0f }));  // Pink
                
                s_TextureMap['-'] = s_SubTextureVectorMap['-'][3];
            }
            
            // PipesHead
            {
                s_SubTextureVectorMap['Y'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 19.0f }, { 2.0f, 1.0f })); // Green
                s_SubTextureVectorMap['Y'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 15.0f }, { 2.0f, 1.0f })); // Orange
                s_SubTextureVectorMap['Y'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 13.0f }, { 2.0f, 1.0f })); // Grey
                s_SubTextureVectorMap['Y'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 9.0f }, { 2.0f, 1.0f }));  // Pink
                
                s_SubTextureVectorMap['!'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 18.0f }, { 2.0f, 1.0f })); // Green
                s_SubTextureVectorMap['!'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 14.0f }, { 2.0f, 1.0f })); // Orange
                s_SubTextureVectorMap['!'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 12.0f }, { 2.0f, 1.0f })); // Grey
                s_SubTextureVectorMap['!'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 8.0f }, { 2.0f, 1.0f }));  // Pink
                
                s_TextureMap['Y'] = s_SubTextureVectorMap['Y'][3];
                s_TextureMap['!'] = s_SubTextureVectorMap['!'][3];
            }
            
            // Adding Texture maps
            // Ground
            
            // Castel
            s_TextureMap['.'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 19.0f, 25.0f });
            s_TextureMap['u'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 20.0f, 24.0f });
            s_TextureMap['o'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 21.0f, 24.0f });
            s_TextureMap['|'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 21.0f, 25.0f });
            s_TextureMap['l'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 20.0f, 25.0f });
            s_TextureMap['r'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 22.0f, 25.0f });
            
            for (uint32_t y = 0; y < mapHeight; y++)
            {
                for (uint32_t x = 0; x < mapWidth; x++)
                {
                    if (char tileType = s_MapTiles[x + y * mapWidth]; s_TextureMap.find(tileType) != s_TextureMap.end())
                    {
                        Ref<SubTexture> subTexture = s_TextureMap[tileType];
                        
                        auto entity = s_EntityVector[tileType].emplace_back(scene->CreateEntity(GetEntityNameFromChar(tileType)));
                        entity.AddComponent<BoxCollider2DComponent>();
                        
                        auto spriteEntity = entity.AddComponent<SpriteRendererComponent>(subTexture);
                        auto spriteSize   = spriteEntity.SubTexComp->GetSpriteSize();
                        
                        auto& tc = entity.GetComponent<TransformComponent>();
                        
                        tc.Translation = { x, (mapHeight / 2.0f) - y, 0.0f };
                        tc.Scale       = { spriteSize.x, spriteSize.y , 0.0f};
                    }
                }
            }
        }
        
        // Camera Entity
        {
            Ref<Scene> scene = m_ActiveScene;
            
            Entity cameraEntity = scene->CreateEntity("Camera");
            auto& camera = cameraEntity.AddComponent<CameraComponent>().Camera;
            camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
            camera.SetOrthographicSize(20.0);
        }
        
        // Player Entity
        {
#define IsCollision(x) (m_Entity.GetScene()->CollisionDetection(m_Entity, speed * ts) & (int)Scene::CollisionSide::x)

            auto playerEntity = m_ActiveScene->CreateEntity("Player");
            playerEntity.AddComponent<SpriteRendererComponent>();
            
            class PlayerController : public ScriptableEntity
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
                        
                        if(Input::IsKeyPressed(Key::Left))
                        {
                            if (!IsCollision(Left))
                            {
                                translation.x -= speed * ts;
                            }
                        }
                        if(Input::IsKeyPressed(Key::Right))
                        {
                            if (!IsCollision(Right))
                            {
                                translation.x += speed * ts;
                            }
                        }
                        if(Input::IsKeyPressed(Key::Up))
                        {
                            if (!IsCollision(Up))
                            {
                                translation.y += speed * ts;
                            }
                        }
                        if(Input::IsKeyPressed(Key::Down))
                        {
                            if (!IsCollision(Down))
                            {
                                translation.y -= speed * ts;
                            }
                        }
                    }
                }
                
                void OnDestroy()
                {
                }
                
                // OnAwake(), CollisionCallbacks()
            };
            
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
        
        Renderer::Clear(s_BgColor);
                    
        m_ActiveScene->OnUpdate(timeStep);
        
        m_FrameBuffer->Unbind();
    }
                    
    void MarioLayer::OnImguiRender()
    {
        ImGuiAPI::EnableDcocking();
        // ----------------------- Menu Bar ---------------------------------------------------------------
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit")) Application::Get().Close();
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
        
        // Stats 
        ImGuiAPI::FrameRate();
        ImGuiAPI::RendererStats();
        ImGuiAPI::RendererVersion();
        m_SceneHierarchyPannel.OnImguiender();
        
        // ----------------------- Setings ----------------------------------------------------------------
        ImGui::Begin("Setting");
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
            
        // Ending of Docking egining
        ImGuiAPI::EndDocking();
    }
                                    
}
