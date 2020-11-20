#pragma once

#include <iKan.h>

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
                float speed = 2.5f;

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
