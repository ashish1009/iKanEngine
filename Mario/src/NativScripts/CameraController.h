#pragma once

#include <iKan.h>
#include <Player.h>

using namespace iKan;

namespace Mario {

    class CameraController : public ScriptableEntity
    {
    public:
        void OnCreate()
        {
        }

        void OnUpdate(TimeStep ts)
        {
            if (HasComponent<TransformComponent>())
            {
                auto& translation = GetComponent<TransformComponent>().Translation;
                float speed = Player::s_Speed;

                if(Input::IsKeyPressed(Key::Left))
                {
                    translation.x -= speed * ts;
                }
                if(Input::IsKeyPressed(Key::Right))
                {
                    translation.x += speed * ts;
                }
            }
        }

        void OnDestroy()
        {
        }

        // OnAwake(), CollisionCallbacks()
    };

}
