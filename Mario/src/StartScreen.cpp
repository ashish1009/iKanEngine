#include "StartScreen.h"

namespace Mario {

    static std::string GetEntityNameFromChar(char type)
    {
        switch(type)
        {
            case 'T' : return "Top Border";     break;
            case 'B' : return "Bottom Border";  break;
            case 'R' : return "Right Border";   break;
            case 'L' : return "Left Border";    break;

            case ' ' : return "Solid Brown";    break;
            case '|' : return "Solid Cream";    break;
            case '#' : return "Solid Unused";    break;

            case '1' : return "Bottom Left Dot Border";     break;
            case '3' : return "Bottom Right Dot Border";    break;
            case '7' : return "Top Left Dot Border";        break;
            case '9' : return "Top Right Dot Border";       break;

            case 'M' : return "Mario Written";       break;

            case 'a' : return "a";       break;
            case 'b' : return "b";       break;
            case 'c' : return "c";       break;
            case 'd' : return "d";       break;
            case 'e' : return "e";       break;
            case 'f' : return "f";       break;
            case 'g' : return "g";       break;
            case 'h' : return "h";       break;
            case 'i' : return "i";       break;
            case 'j' : return "j";       break;

            case ',' : return ",";       break;
            case '/' : return "/";       break;
            case '?' : return "?";       break;

            case 'k' : return "k";       break;
            case 'l' : return "l";       break;
            case 'm' : return "m";       break;
            case 'n' : return "n";       break;
            case 'o' : return "o";       break;
            case 'p' : return "p";       break;
            case 'q' : return "q";       break;
            case 'r' : return "r";       break;
            case 's' : return "s";       break;
            case 't' : return "t";       break;
            case 'u' : return "u";       break;
            case 'v' : return "v";       break;
            case 'w' : return "w";       break;
            case 'x' : return "x";       break;
            case 'y' : return "y";       break;
            case 'z' : return "z";       break;
        }
        IK_ASSERT(false, "Invalid Type");
        return "";
    }

    static const std::string s_BasicPath = "/Users/ashish./iKan/Github/iKanEngine/Mario/assets/Resources/Graphics/StartScreen/";

    static std::string s_CharTiles =
    "7TTTTTTTTTTTTTTTTTTTTTTTT90"
    "L                        R0"
    "L  i vvklkli kjlklylikl  R0"
    "L  v vvv,v,v v,vv,v,vv,  R0"
    "L  v vavvvvv vvvvvvavvv  R0"
    "L  v vsy/vvv vvvy/vsvvv  R0"
    "L  v vvv?vot vvvv?vvvot  R0"
    "1BBcBcccccqrBccccccccqrBB30"
    ;

    Ref<Texture> StartScreen::s_TileSpriteSheet = nullptr;

    std::unordered_map<char, Ref<SubTexture>> StartScreen::s_CharSubTextureMap;

    void StartScreen::Init(Ref<Scene>& scene)
    {
        s_TileSpriteSheet = Texture::Create(s_BasicPath + "Tile.png");

        {
            s_CharSubTextureMap['a'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 1.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['b'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 1.0f, 1.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['c'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 2.0f, 1.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['d'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 3.0f, 1.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['e'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 4.0f, 1.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['f'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 5.0f, 1.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['g'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 6.0f, 1.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['h'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 7.0f, 1.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['i'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 8.0f, 1.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['j'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 9.0f, 1.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });

            s_CharSubTextureMap[','] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 10.0f, 1.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['/'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 11.0f, 1.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['?'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 12.0f, 1.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });

            s_CharSubTextureMap['k'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 2.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['l'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 1.0f, 2.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['m'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 2.0f, 2.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['n'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 3.0f, 2.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['o'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 4.0f, 2.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['p'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 5.0f, 2.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['q'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 6.0f, 2.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['r'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 7.0f, 2.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['s'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 8.0f, 2.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['t'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 9.0f, 2.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['u'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 10.0f, 2.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['v'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 11.0f, 2.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['w'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 12.0f, 2.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['x'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 13.0f, 2.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['y'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 14.0f, 2.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['z'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 15.0f, 2.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });

            s_CharSubTextureMap['7'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 0.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['9'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 1.0f, 0.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['1'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 2.0f, 0.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['3'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 3.0f, 0.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });

            s_CharSubTextureMap['L'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 4.0f, 0.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['B'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 5.0f, 0.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['T'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 6.0f, 0.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['R'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 7.0f, 0.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });

            s_CharSubTextureMap['|'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 8.0f, 0.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap[' '] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 9.0f, 0.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
            s_CharSubTextureMap['#'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 10.0f, 0.0f }, { 1.0f, 1.0f } , { 8.0f, 8.0f });
        }

        auto mapWidth = s_CharTiles.find_first_of('0') + 1;
        uint32_t mapHeight = static_cast<uint32_t>(strlen(s_CharTiles.c_str())) / mapWidth;

        for (uint32_t y = 0; y < mapHeight; y++)
        {
            for (uint32_t x = 0; x < mapWidth; x++)
            {
                if (char tileType = s_CharTiles[x + y * mapWidth]; s_CharSubTextureMap.find(tileType) != s_CharSubTextureMap.end())
                {
                    auto entity = scene->CreateEntity(GetEntityNameFromChar(tileType));
                    auto spriteEntity = entity.AddComponent<SpriteRendererComponent>(s_CharSubTextureMap[tileType]);

                    auto& tc = entity.GetComponent<TransformComponent>();

                    tc.Translation = { x + 7, (mapHeight / 2.0f) - y, 0.0f };
                } //if (char tileType = s_MapTiles[x + y * mapWidth]; s_TextureMap.find(tileType) != s_TextureMap.end())
            } // for (uint32_t x = 0; x < mapWidth; x++)
        } // for (uint32_t y = 0; y < mapHeight; y++)
    }

}



