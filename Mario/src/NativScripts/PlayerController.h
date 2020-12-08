#pragma once

#include <iKan.h>
#include <Player.h>

#define IsCollision(x) (m_Entity.GetScene()->CollisionDetection(m_Entity, speed) & (int)Scene::CollisionSide::x)

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
                auto& position = Player::Get().GetPosition();
                float speed = Player::Get().GetTranslationSpeed();

                if(Input::IsKeyPressed(Key::Left))
                {
                    if (!IsCollision(Left))
                    {
                        position.x -= speed;
                    }
                }
                if(Input::IsKeyPressed(Key::Right))
                {
                    if (!IsCollision(Right))
                    {
                        position.x += speed;
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
