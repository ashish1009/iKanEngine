#pragma once

#include <iKan.h>

using namespace iKan;

namespace Mario {

    class Player
    {
    public:
        Player(const Player& other) = delete;
        Player(Player&& other) = delete;
        Player& operator =(const Player& other) = delete;
        Player& operator =(Player&& other) = delete;

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

        void SetSpeed(float speed) { m_Speed = speed; }

        float GetSpeed() const { return m_Speed; }

    private:
        Player() = default;
        ~Player() = default;

    private:
        static Player* s_Instance;

        Entity m_Entity;
        float m_Speed = 12.5f;
    };

}
