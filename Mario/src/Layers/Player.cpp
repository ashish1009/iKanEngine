#include "Player.h"

namespace iKan {
    
    Player* Player::s_Instance = nullptr;
    
    Player::Player(const std::shared_ptr<Scene>& scene)
    {
        IK_INFO("Player Instance Created");
        
        m_PlayerSpriteSheet = Texture::Create("../../Mario/assets/Resources/Graphics/Player.png");
        
        m_PlayerSubtexture = SubTexture::CreateFromCoords(m_PlayerSpriteSheet, { 0, 12 });
                
        m_PlayerEntity = scene->CreateEntity("Player");
        m_PlayerEntity.AddComponent<SpriteRendererComponent>(m_PlayerSubtexture);
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
