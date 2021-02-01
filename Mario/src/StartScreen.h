#pragma once

#include <iKan.h>

using namespace iKan;

namespace Mario {

    class StartScreen
    {
    public:
        static void Init(Ref<Scene>& scene);

    public:
        static Ref<Texture> s_TileSpriteSheet;

        static std::unordered_map<char, Ref<SubTexture>> s_CharSubTextureMap;
    };

}
