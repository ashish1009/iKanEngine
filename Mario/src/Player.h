// ******************************************************************************
//   File    : Player.h
//   Project : Mario
//
//   Created by Ashish
// ******************************************************************************
#pragma once

#include <iKan.h>

namespace Mario {
    
    using namespace iKan;
    class PlayerController;
    
    // ******************************************************************************
    // Class to store the player infrmation
    // ******************************************************************************
    class Player
    {
    public:
        // There are total of 11 color available for player
        static constexpr int32_t MaxPlayerImages    = 11;
        
        // as there is difference of 3 between 2 color of player in tile Texture.
        // 2 for Biigger player and 1 for smaller player
        static constexpr int32_t PlayerImgColorDiff = 3;
        
        // For running we have 3 images to rotate
        static constexpr int32_t MaxPlayerRunImages = 3;
        
        // These are speed factor. Not frame time dependent
        static constexpr float TranslationSpeed = 6.0f;
        static constexpr float JumpSpeed        = 12.5;
        static constexpr float FreeFallSpeed    = 25.0f;

    public:
        ~Player() = default;
        
        static Player& Get()
        {
            if (!s_Instance)
                s_Instance = new Player();
            return *s_Instance;
        }
        
        void OnInit(Ref<Scene> scene);
        void OnUpdate(TimeStep ts);
        void OnImGuiRenderer();
        
    private:
        Player() = default;
        
        Player(const Player& other) = delete;
        Player(Player&& other)      = delete;

        Player& operator =(const Player& other) = delete;
        Player& operator =(Player&& other)      = delete;
        
        void OnKeyPressed(KeyPressedEvent& event);
        void OnKeyReleased(KeyReleasedEvent& event);

    private:
        static Player* s_Instance;
        
        Ref<Texture>   m_SpriteSheet;
        Entity         m_Entity;
        
        // Subtexture of player stored as per its state
        Ref<SubTexture> m_StandSubtexture[MaxPlayerImages];
        Ref<SubTexture> m_JumpSubtexture[MaxPlayerImages];
        Ref<SubTexture> m_RunningSubtexture[MaxPlayerImages][MaxPlayerRunImages];
        
        // Speed Variables: These speeds are time step dependent
        float m_TranslationSpeed;
        float m_FreeFallSpeed;
        float m_JumpSpeed;
        
        friend class PlayerController;
    };
    
}
