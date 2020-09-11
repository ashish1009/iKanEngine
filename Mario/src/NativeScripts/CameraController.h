#pragma once

#include <Layers/Player.h>

namespace iKan {
    
#define CAMERA_START 15

    class MarioCamera;

    class CameraController : public ScriptableEntity
    {
    public:
        void OnUpdate(TimeStep ts)
        {
            auto& x = MarioCamera::GetPositionX();
            
            auto player             = Player::Get();
            auto[playerX, playerY]  = player->GetPosition();
            float speed             = player->GetSpeed();
            
            if (Input::IsKeyPressed(Key::Left) && x > CAMERA_START)
                x -= speed * ts;
            if(Input::IsKeyPressed(Key::Right) && playerX > 10)
                x += speed * ts;
        }
    };
    
}
