#pragma once

#include <iKan.h>

using namespace iKan;

namespace Mario {

    class Player
    {
    public:
        static constexpr float TranslationSpeed = 12.5;
        static constexpr float JumpSpeed        = 12.5;
        static constexpr float FreeFallSpeed    = 12.5;

    public:
        enum class State
        {
            Default = BIT(0),
            Falling = BIT(1),
            Jumping = BIT(2),
        };

    public:
        Player(const Player& other) = delete;
        Player(Player&& other)      = delete;

        Player& operator =(const Player& other) = delete;
        Player& operator =(Player&& other)      = delete;

        static Player& Get()
        {
            if (!s_Instance)
                s_Instance = new Player();
            return *s_Instance;
        }

        void Init(Ref<Scene>& scene);
        void Update(TimeStep ts);
        void OnKeyPressed(KeyPressedEvent& event);
        void OnKeyReleased(KeyReleasedEvent& event);

        void SetTranslationSpeed(float speed) { m_TranslationSpeed = speed; }
        void SetFreeFallSpeed(float speed) { m_FreeFallSpeed = speed; }
        void SetJumpSpeed(float speed) { m_JumpSpeed = speed; }

        float GetTranslationSpeed() const { return m_TranslationSpeed; }
        float GetFreeFallSpeed() const { return m_FreeFallSpeed; }
        float GetJumpSpeed() const { return m_JumpSpeed; }

        void FreeFall();
        void Stand();
        void Jump();

        int32_t ToggleBit(int32_t bitPos);
        int32_t ClearBit(int32_t bitPos);
        int32_t SetBit(int32_t bitPos);

        void StateCallbacks(State state);

    private:
        Player() = default;
        ~Player() = default;

    private:
        static Player* s_Instance;

        // 3 : Max number of states for now
        std::array<void (Player::*)(), 3> m_PlayerUpdateFnPtr;

        int32_t m_State = (int32_t)(State::Falling);

        float m_TranslationSpeed = 1.0f;
        float m_FreeFallSpeed    = 1.0f;
        float m_JumpSpeed        = 1.0f;
        
        Entity m_Entity;
    };

}
