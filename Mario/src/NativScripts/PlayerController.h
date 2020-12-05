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
                float speed = Player::Get().GetTranslationSpeed();

                if(Input::IsKeyPressed(Key::Left))
                {
                    if (!IsCollision(Left))
                    {
                        translation.x -= speed;
                    }
                }
                if(Input::IsKeyPressed(Key::Right))
                {
                    if (!IsCollision(Right))
                    {
                        translation.x += speed;
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
