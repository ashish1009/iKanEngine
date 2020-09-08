#pragma once

namespace iKan {

    class MarioCamera;
    class Player;

    class CameraController : public ScriptableEntity
    {
    public:
        void OnUpdate(TimeStep ts)
        {
            auto& transform = GetComponent<TransformComponent>().Transform;
            float speed = MarioCamera::s_Speed;
            
            auto& position = transform[3];
            if(Input::IsKeyPressed(Key::Left))
                position[0] -= speed * ts;
            if(Input::IsKeyPressed(Key::Right))
                position[0] += speed * ts;
        }
    };
    
}
