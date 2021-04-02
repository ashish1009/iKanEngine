// ******************************************************************************
//   File    : Background.h
//   Project : Mario
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <iKan.h>

using namespace iKan;

namespace Mario {

    // ******************************************************************************
    // Class to store and render Background tiles
    // ******************************************************************************
    class BackgroundTile
    {
    public:
        static void CreateEntities(Ref<Scene>& scene);
        static void ImGuiRenderer();
        
    private:
        static void Init();
        static void ImgButtons(const char name);

    public:
        // ******************************************************************************
        // Tile wrapper from background
        // ******************************************************************************
        struct Tile
        {
            Ref<iKan::SubTexture> SubTexture;
            bool IsRigid = true;
            bool IsProp  = false;
        };

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

