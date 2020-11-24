#pragma once

#include <iKan.h>
#include <Player.h>

#define IsCollision(x) (m_Entity.GetScene()->CollisionDetection(m_Entity, speed * ts) & (int)Scene::CollisionSide::x)

using namespace iKan;

namespace Mario {

    class PlayerController : public ScriptableEntity
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
                    if (!IsCollision(Left))
                    {
                        translation.x -= speed * ts;
                    }
                }
                if(Input::IsKeyPressed(Key::Right))
                {
                    if (!IsCollision(Right))
                    {
                        translation.x += speed * ts;
                    }
                }
            }
        }

        void OnDestroy()
        {
        }

        // OnAwake(), CollisionCallbacks()
    };

}
