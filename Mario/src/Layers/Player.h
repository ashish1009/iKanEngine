#pragma once

#include <iKan.h>

namespace iKan {
    
    // For Big Player Add the value by 1
    enum class PlayerColor
    {
        Black = 0, Skin = 3, Grey = 6, Blue = 9, BlueOrange = 12, RedOrange = 15, GreenOrange = 18, BlackOrange = 21, GreeWhite = 24, RedWhite = 27, Basic = 30,
    };
    
    class PlayerMove;
    
    class Player
    {
    public:
        ~Player();
        
        static Player* Create(const std::shared_ptr<Scene>& scene);
        static Player* Get() { IK_ASSERT(s_Instance, "Player didnt existed !!!"); return s_Instance; }
        static void Destroy() { if (s_Instance) delete s_Instance; }
        
        bool OnKeyPressed(KeyPressedEvent& event);
        bool OnKeyReleased(KeyReleasedEvent& event);
        
        void ImGuiRender();
        
        float GetTranslationSpeed() const { return m_TranslationSpeed; }
        float GetLandingSpeed() const { return m_LandingSpeed; }
        
        std::pair<float&, float&> GetPosition()
        {
            auto& position = m_Entity.GetComponent<TransformComponent>().Transform[3];
            return { position[0], position[1] };
        }
        
        float& GetPositionX()
        {
            auto[x, y] = GetPosition();
            return x;
        }
        
        float& GetPositionY()
        {
            auto[x, y] = GetPosition();
            return y;
        }
        
    private:
        Player(const std::shared_ptr<Scene>& scene);
                
    private:
        static Player* s_Instance;
        
        Entity m_Entity;
        std::shared_ptr<SubTexture> m_StandSubtexture;
        std::shared_ptr<Texture> m_SpriteSheet;
        
        bool m_bIsLanded         = false;
        bool m_bIsRightCollision = false;
        bool m_bIsLeftCollision  = false;
        
        float m_Color            = static_cast<float>(PlayerColor::BlueOrange);
        float m_MoveIdx          = 0.0f;
        float m_TranslationSpeed = 10.0f;
        float m_LandingSpeed     = 15.0f;
        float m_JumpingSpeed     = 10.0f;
        
        friend class PlayerMove;
    };
    
}
