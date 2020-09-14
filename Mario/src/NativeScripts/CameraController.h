#pragma once

#include <Layers/Player.h>
#include <Layers/MarioCamera.h>

#define CAMERA_START 15
#define PLAYER_CENTER 15

namespace iKan {
    
    class CameraController : public ScriptableEntity
    {
    public:
        void OnUpdate(TimeStep ts)
        {
            auto& x = MarioCamera::GetPositionX();
            
            auto player  = Player::Get();
            auto playerX = player->GetPositionX();
            float speed  = player->GetSpeed();
            
            if (Input::IsKeyPressed(Key::Left) && x > CAMERA_START)
                x -= speed * ts;
            if(Input::IsKeyPressed(Key::Right) && playerX > PLAYER_CENTER)
                // TODO: Add camera move end limit
                x += speed * ts;
        }
    };
    
}
