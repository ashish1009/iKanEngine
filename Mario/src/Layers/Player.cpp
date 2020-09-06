#include "Player.h"

namespace iKan {
    
    Player* Player::s_Instance = nullptr;
    
    static std::shared_ptr<Texture> s_PlayerSpriteSheet;

    Player::Player(const std::shared_ptr<Scene>& scene)
    {
        IK_INFO("Player Instance Created");
        
        s_PlayerSpriteSheet = Texture::Create("../../Mario/assets/Resources/Graphics/Player.png");
        
        m_PlayerSubtexture = SubTexture::CreateFromCoords(s_PlayerSpriteSheet, { 6.0f, 12.0f });
                
        m_PlayerEntity = scene->CreateEntity("Player");
        m_PlayerEntity.AddComponent<SpriteRendererComponent>(m_PlayerSubtexture);
        
        class PlayerMove : public ScriptableEntity
        {
        public:
            void OnUpdate(TimeStep timestep)
            {
                if (Input::IsKeyPressed(Key::Right))
                {
                    static float moveIdx = 0.0f;
                    auto playerTexture = SubTexture::CreateFromCoords(s_PlayerSpriteSheet, { (float)((uint32_t)moveIdx), 12.0f });
                    Player::Get()->m_PlayerEntity.GetComponent<SpriteRendererComponent>().SubTexComp = playerTexture;
                    moveIdx += (timestep * 10);
                    if (moveIdx > 3.0f)
                        moveIdx = 0.0f;
                }
                
                if (Input::IsKeyReleased(Key::Right))
                {
                    auto playerTexture = Player::Get()->m_PlayerSubtexture;
                    Player::Get()->m_PlayerEntity.GetComponent<SpriteRendererComponent>().SubTexComp = playerTexture;
                }
            }
        };
        
        m_PlayerEntity.AddComponent<NativeScriptComponent>().Bind<PlayerMove>();
    }
    
    Player::~Player()
    {
        IK_WARN("Player Instance Destroyed");
    }
    
    Player* Player::Create(const std::shared_ptr<Scene>& scene)
    {
        if (!s_Instance)
            s_Instance = new Player(scene);
        return s_Instance;
    }
    
    void Player::Destroy()
    {
        if (s_Instance)
            delete s_Instance;
    }
    
}
