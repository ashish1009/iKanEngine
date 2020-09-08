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
        static void Init (const std::shared_ptr<Scene>& scene);
                
    private:
        static Entity s_Entity;
        static std::shared_ptr<SubTexture> s_StandSubtexture;
        static std::shared_ptr<Texture> s_SpriteSheet;
        static float s_Color;
        
        friend class PlayerMove;
    };
    
}
