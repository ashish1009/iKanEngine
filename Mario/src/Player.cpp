// ******************************************************************************
//   File    : Player.cpp
//   Project : Mario
//
//   Created by Ashish
// ******************************************************************************

#include "Player.h"
#include "PlayerController.h"

namespace Mario {
    
    Player* Player::s_Instance = nullptr;
    
    // ******************************************************************************
    // Initialise the Player entity
    // ******************************************************************************
    void Player::OnInit(Ref<Scene> scene)
    {
        IK_INFO("Player Instance Created");
        m_SpriteSheet = Texture::Create("../../Mario/assets/Resources/Graphics/Player.png");
        
        // Storing all kind of images in init time
        for (int32_t playerImgIdx = 0; playerImgIdx < MaxPlayerImages; playerImgIdx++)
        {
            m_StandSubtexture[playerImgIdx] = SubTexture::CreateFromCoords(m_SpriteSheet, { 6.0f, float(playerImgIdx * PlayerImgColorDiff) + 1.0f }, { 1.0f, 2.0f });
            m_JumpSubtexture[playerImgIdx]  = SubTexture::CreateFromCoords(m_SpriteSheet, { 4.0f, float(playerImgIdx * PlayerImgColorDiff) });

            for (int32_t runImgIdx = 0; runImgIdx < MaxPlayerRunImages; runImgIdx++)
            {
                m_RunningSubtexture[playerImgIdx][runImgIdx] = SubTexture::CreateFromCoords(m_SpriteSheet, { float(runImgIdx), float(playerImgIdx * PlayerImgColorDiff) });
            }
        }
        
        // Creating Player Entity
        m_Entity = scene->CreateEntity("Player");
        m_Entity.AddComponent<SpriteRendererComponent>();
        m_Entity.AddComponent<NativeScriptComponent>().Bind<PlayerController>();
    }
    
    // ******************************************************************************
    // Opdate the player entity for each frame
    // ******************************************************************************
    void Player::OnUpdate(TimeStep ts)
    {
        m_TranslationSpeed = TranslationSpeed * ts;
        m_FreeFallSpeed    = FreeFallSpeed * ts;
        m_JumpSpeed        = JumpSpeed * ts;
    }
    
    // ******************************************************************************
    // Render imgui for player each frame
    // ******************************************************************************
    void Player::OnImGuiRenderer()
    {
    }
    
    // ******************************************************************************
    // Key press event for player
    // ******************************************************************************
    void Player::OnKeyPressed(KeyPressedEvent &event)
    {
    }
    
    // ******************************************************************************
    // Key release event for player
    // ******************************************************************************
    void Player::OnKeyReleased(KeyReleasedEvent &event)
    {
    }
    
}
