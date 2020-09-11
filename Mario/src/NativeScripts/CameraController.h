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
            auto[x, y] = MarioCamera::GetPosition();
            float speed = Player::Get()->GetSpeed();
            
            if (Input::IsKeyPressed(Key::Left) && x > CAMERA_START)
                x -= speed * ts;
            if(Input::IsKeyPressed(Key::Right))
                x += speed * ts;
        }
    };
    
}
