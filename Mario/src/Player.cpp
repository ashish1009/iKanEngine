#include "Player.h"

namespace iKan {
    
    Player* Player::s_Instance = nullptr;
    
    Player::Player()
    {
        IK_INFO("Player Instance Created");
    }
    
    Player::~Player()
    {
        IK_WARN("Player Instance Destroyed");
    }
    
    Player* Player::Create()
    {
        if (!s_Instance)
            s_Instance = new Player();
        return s_Instance;
    }
    
    void Player::Destroy()
    {
        if (s_Instance)
            delete s_Instance;
    }
    
}
