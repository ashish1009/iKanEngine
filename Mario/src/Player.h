#pragma once

namespace iKan {
    
    class Player
    {
    public:
        Player();
        ~Player();
                
        static Player* Create();
        static void Destroy();
        static Player* Get() { return s_Instance; }
        
    private:
        static Player* s_Instance;
    };
    
}
