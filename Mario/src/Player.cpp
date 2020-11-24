#include "Player.h"

#include "NativScripts/PlayerController.h"

namespace Mario {

    Entity Player::s_Entity;
    float Player::s_Speed = 12.5f;

    void Player::Init(Ref<Scene>& scene)
    {
        s_Entity = scene->CreateEntity("Player");
        s_Entity.AddComponent<SpriteRendererComponent>();
        s_Entity.AddComponent<NativeScriptComponent>().Bind<PlayerController>();
    }

    void Player::Update(TimeStep ts)
    {
        auto& position = s_Entity.GetComponent<TransformComponent>().Translation;
        float speed = s_Speed * ts;

        if (!(s_Entity.GetScene()->CollisionDetection(s_Entity, speed) & (int)Scene::CollisionSide::Down))
        {
            position.y -= speed;
        }
    }

}
