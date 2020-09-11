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
        
    private:
        Player(const std::shared_ptr<Scene>& scene);
                
    private:
        static Player* s_Instance;
        
        Entity m_Entity;
        std::shared_ptr<SubTexture> m_StandSubtexture;
        std::shared_ptr<Texture> m_SpriteSheet;
        float m_Color = static_cast<float>(PlayerColor::BlueOrange);
        float m_MoveIdx = 0.0f;
        
        friend class PlayerMove;
    };
    
}
