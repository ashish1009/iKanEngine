#include "Player.h"

#include "NativScripts/PlayerController.h"

namespace Mario {

    Player* Player::s_Instance = nullptr;

    static inline size_t GetBitPos(int32_t state)
    {
        if (!state)
            return 0;
        return log2(state);
    }

    int32_t Player::ToggleBit(int32_t state)
    {
        return (m_State ^ (1 << (int32_t)log2(state)));
    }

    int32_t Player::ClearBit(int32_t state)
    {
        return (m_State & (~(1 << (int32_t)log2(state))));
    }

    int32_t Player::SetBit(int32_t state)
    {
        return (m_State | state);
    }

    void Player::Init(Ref<Scene>& scene)
    {
        m_Entity = scene->CreateEntity("Player");
        m_Entity.AddComponent<SpriteRendererComponent>();
        m_Entity.AddComponent<NativeScriptComponent>().Bind<PlayerController>();

        // Registering the callback function according to the states
        m_PlayerUpdateFnPtr[GetBitPos((int32_t)State::Default)] = &Player::Stand;
        m_PlayerUpdateFnPtr[GetBitPos((int32_t)State::Falling)] = &Player::FreeFall;
        m_PlayerUpdateFnPtr[GetBitPos((int32_t)State::Jumping)] = &Player::Jump;
    }

    void Player::StateCallbacks(State state)
    {
        size_t index = GetBitPos(m_State & (int32_t)state);
        if (index)
            (this->*m_PlayerUpdateFnPtr[index])();
    }

    void Player::Update(TimeStep ts)
    {
        m_TranslationSpeed = TranslationSpeed * ts;
        m_FreeFallSpeed    = FreeFallSpeed * ts;
        m_JumpSpeed        = JumpSpeed * ts;

        // Update the states
        {
            if ((m_Entity.GetScene()->CollisionDetection(m_Entity, m_FreeFallSpeed) & (int)Scene::CollisionSide::Down))
            {
                m_State = ClearBit((int32_t)State::Falling);
                m_State = SetBit((int32_t)State::Default);
            }
            else
            {
                if (!(m_State & (int32_t)State::Jumping))
                {
                    m_State = SetBit((int32_t)State::Falling);
                }
            }
        }

        StateCallbacks(State::Default);
        StateCallbacks(State::Falling);
        StateCallbacks(State::Jumping);

        IK_CORE_INFO("STATE {0}", m_State);
    }

    void Player::OnKeyPressed(KeyPressedEvent& event)
    {
        if (Key::X == event.GetKeyCode())
        {
            m_State = SetBit((int32_t)State::Jumping);
            m_State = ClearBit((int32_t)State::Falling);
        }
    }

    void Player::OnKeyReleased(KeyReleasedEvent& event)
    {

    }

    void Player::FreeFall()
    {
        auto& position = m_Entity.GetComponent<TransformComponent>().Translation;
        position.y -= m_FreeFallSpeed;
    }

    void Player::Stand()
    {

    }

    void Player::Jump()
    {
        auto& position = m_Entity.GetComponent<TransformComponent>().Translation;
        static float jumpHeight = position.y;

        if (position.y - jumpHeight < 3)
        {
            position.y += m_JumpSpeed;
            IK_CORE_INFO("Position {0}, {1}, {2}, {3}", position.y, jumpHeight, (position.y - jumpHeight), m_JumpSpeed);
        }
        else
        {
            m_State = ClearBit((int32_t)State::Jumping);
        }
    }

}
