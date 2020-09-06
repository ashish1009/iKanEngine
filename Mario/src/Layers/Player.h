#pragma once

#include <iKan.h>

namespace iKan {
    
    class PlayerMove;
    
    class Player
    {
    public:
        Player(const std::shared_ptr<Scene>& scene);
        ~Player();
                
        static Player* Create(const std::shared_ptr<Scene>& scene);
        static void Destroy();
        static Player* Get() { return s_Instance; }
        
    private:
        static Player* s_Instance;
        
        std::shared_ptr<SubTexture> m_PlayerSubtexture;
        
        Entity m_PlayerEntity;
        
        friend class PlayerMove;
    };
    
}
