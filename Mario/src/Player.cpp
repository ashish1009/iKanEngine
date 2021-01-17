#include "Player.h"

#include "NativScripts/PlayerController.h"

namespace Mario {

#define ClearState(state)           m_State = (m_State & (~(1 << (int32_t)log2((int32_t)State::state))))
#define AddState(state)             m_State = (m_State | (int32_t)State::state)

    Player* Player::s_Instance = nullptr;

    static int32_t GetBitPos(int32_t state)
    {
        if (!state)
            return -1;
        return log2(state);
    }

    void Player::StateCallback(State state)
    {
        int32_t index = GetBitPos(m_State & (int32_t)state);
        if (index >= 0)
            (this->*m_PlayerStateFnPtr[index])();
    }

    void Player::Init(Ref<Scene>& scene)
    {
        m_Entity = scene->CreateEntity("Player");
        m_Entity.AddComponent<SpriteRendererComponent>();
        m_Entity.AddComponent<NativeScriptComponent>().Bind<PlayerController>();

        m_Position = m_Entity.GetComponent<TransformComponent>().Translation;

        // Registering the callback function according to the states
        m_PlayerStateFnPtr[GetBitPos((int32_t)State::Standing)] = &Player::Stand;
        m_PlayerStateFnPtr[GetBitPos((int32_t)State::Falling)]  = &Player::Freefall;
        m_PlayerStateFnPtr[GetBitPos((int32_t)State::Jumping)]  = &Player::Jump;
    }

    void Player::Update(TimeStep ts)
    {
        m_TranslationSpeed = TranslationSpeed * ts;
        m_FreeFallSpeed    = FreeFallSpeed * ts;
        m_JumpSpeed        = JumpSpeed * ts;

        {
            if (IsCollision(Down, m_FreeFallSpeed))
            {
                ClearState(Falling);
                AddState(Standing);
            }
            else
            {
                ClearState(Standing);
                AddState(Falling);
            }
        }

        StateCallback(State::Standing);
        StateCallback(State::Falling);
        StateCallback(State::Jumping);

        // Update the local position to the entity
        m_Entity.GetComponent<TransformComponent>().Translation = m_Position;
    }

    void Player::Stand()
    {
        m_Position.y = std::floor(m_Position.y);
    }

    void Player::Freefall()
    {
        m_Position.y -= m_FreeFallSpeed;
    }

    void Player::Jump()
    {

    }

    void Player::OnKeyPressed(KeyPressedEvent& event)
    {
        if (Key::X == event.GetKeyCode())
        {
        }
    }

    void Player::OnKeyReleased(KeyReleasedEvent& event)
    {
    }

}
