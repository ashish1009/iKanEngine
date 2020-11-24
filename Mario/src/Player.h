#pragma once

#include <iKan.h>

using namespace iKan;

namespace Mario {

    class Player
    {
    public:
        static void Init(Ref<Scene>& scene);
        static void Update(TimeStep ts);

    public:
        static Entity s_Entity;

        static float s_Speed;
    };

}
