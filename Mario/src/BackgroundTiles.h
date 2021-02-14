#pragma once

#include <iKan.h>

using namespace iKan;

namespace Mario {

    class BackgroundTile
    {
    public:
        struct Tile
        {
            Ref<iKan::SubTexture> SubTexture;
            bool Rigid = true;
        };

    public:
        static void Init(Ref<Scene>& scene);
        static void ImGuiRenderer();
        static void ImgButtons(const char name);

        static void ChangeBonusTextureToUsed(Entity entity);
        static void OnCollision(Scene::CollisionSide collisionSide, Entity colloidedEntity);

    public:
        static glm::vec4    s_BgColor;
        static Ref<Texture> s_TileSpriteSheet;

        // Map of Subtexture to the Char of their corresponding
        static std::unordered_map<char, Tile> s_TextureMap;

        // Map of group of Entity to the Char of their corresponding
        static std::unordered_map<char, std::vector<Entity>> s_EntityVector;

        // Map of gorup of Subtextures to the Char of their corresponding
        static std::unordered_map<char, std::vector<Ref<SubTexture>>> s_SubTextureVectorMap;

    };

}
