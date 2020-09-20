#include <Layers/Background.h>
#include <Layers/SceneMap.h>

namespace iKan {

    static std::shared_ptr<Texture> s_TileSpriteSheet;
    static std::unordered_map<char, std::shared_ptr<SubTexture>> s_TextureMap;
    static std::unordered_map<char, std::vector<Entity>> s_EntityVector;
    static std::unordered_map<char, std::vector<std::shared_ptr<SubTexture>>> s_SubTextureVectorMap;
    
    glm::vec4 Background::s_BgColor = { 0.3f, 0.1f, 0.6f, 1.0f };
    
    static std::string GetEntityNameFromChar(char type)
    {
        switch(type)
        {
            case 'G' : return "Ground";              break;
            case '|' : return "Castel Brick";        break;
            case 'o' : return "Castel Gate";         break;
            case 'u' : return "castel Gate Domb";    break;
            case '.' : return "Castel Domb";         break;
            case 'l' : return "Castel Windlow Left"; break;
            case 'r' : return "Castel Window Right"; break;
            case 'S' : return "Steps";               break;
            case '-' : return "Bridge";              break;
            case '!' : return "Pipe Base";           break;
            case 'Y' : return "Pipe Head";           break;
            case 'X' : return "Bricks";              break;
            case 'B' : return "Bonus";               break;
        }
        IK_ASSERT(false, "Invalid Type");
        return "";
    }

    static void ImgButtons(const char name)
    {
#pragma clang diagnostic ignored "-Wformat-security"
        if (ImGui::TreeNode(GetEntityNameFromChar(name).c_str()))
        {
            ImTextureID myTexId = (ImTextureID)((size_t)s_TileSpriteSheet->GetRendererID());
            float myTexW = (float)s_TileSpriteSheet->GetWidth();
            float myTexH = (float)s_TileSpriteSheet->GetHeight();
            
            int32_t i = 0;
            for (auto subTex : s_SubTextureVectorMap[name])
            {
                ImGui::PushID(i);
                
                auto coords = subTex->GetCoords();
                glm::vec2 uv1 = { (coords.x + 1) * 16.0f, coords.y * 16.0f };
                glm::vec2 uv0 = { coords.x * 16.0f, (coords.y + 1) * 16.0f };
                if (ImGui::ImageButton(myTexId, ImVec2(32.0f, 32.0f), ImVec2(uv0.x / myTexW, uv0.y / myTexH), ImVec2(uv1.x / myTexW, uv1.y / myTexH), 0))
                    for (auto entity : s_EntityVector[name])
                        if (auto &subTexComp = entity.GetComponent<SpriteRendererComponent>().SubTexComp)
                            subTexComp = subTex;
                
                ImGui::PopID();
                ImGui::SameLine();
                i++;
            }
            ImGui::NewLine();
            ImGui::TreePop();
        }
    }
    
    void Background::Init(const std::shared_ptr<Scene>& scene)
    {
        // Texture tile
        s_TileSpriteSheet = Texture::Create("../../Mario/assets/Resources/Graphics/Tile.png");
        
        uint32_t mapWidth  = s_MapWidth;
        uint32_t mapHeight = static_cast<uint32_t>(strlen(s_MapTiles)) / s_MapWidth;
        
        // Ground SubTexutres
        {
            s_SubTextureVectorMap['G'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 27.0f })); // Brown
            s_SubTextureVectorMap['G'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 25.0f })); // Blue
            s_SubTextureVectorMap['G'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 23.0f })); // Grey
            s_SubTextureVectorMap['G'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 21.0f })); // Green
            
            s_TextureMap['G'] = s_SubTextureVectorMap['G'][3];
        }
        
        // Bricks SubTextures
        {
            s_SubTextureVectorMap['X'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 17.0f, 27.0f })); // Brown;
            s_SubTextureVectorMap['X'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 17.0f, 25.0f })); // Blue;
            s_SubTextureVectorMap['X'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 17.0f, 23.0f })); // Grey;
            s_SubTextureVectorMap['X'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 17.0f, 21.0f })); // green;
            
            s_TextureMap['X'] = s_SubTextureVectorMap['X'][1];
        }
        
        // Bomus SubTextures
        {
            s_SubTextureVectorMap['B'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 24.0f, 27.0f })); // Brown
            s_SubTextureVectorMap['B'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 24.0f, 25.0f })); // Blue
            s_SubTextureVectorMap['B'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 24.0f, 23.0f })); // Grey
            s_SubTextureVectorMap['B'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 24.0f, 21.0f })); // Green
            
            s_TextureMap['B'] = s_SubTextureVectorMap['B'][3];
        }
        
        // Steos SubTextures
        {
            s_SubTextureVectorMap['S'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 26.0f })); // Brown
            s_SubTextureVectorMap['S'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 24.0f })); // Blue
            s_SubTextureVectorMap['S'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 22.0f })); // Grey
            s_SubTextureVectorMap['S'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 20.0f })); // Green
            
            s_TextureMap['S'] = s_SubTextureVectorMap['S'][2];
        }
        
        // Bridge SubTexture
        {
            s_SubTextureVectorMap['-'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 15.0f, 19.0f })); // Green
            s_SubTextureVectorMap['-'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 15.0f, 15.0f })); // Orange
            s_SubTextureVectorMap['-'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 15.0f, 11.0f })); // Grey
            s_SubTextureVectorMap['-'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 15.0f, 9.0f }));  // Pink
            
            s_TextureMap['-'] = s_SubTextureVectorMap['-'][3];
        }
        
        // PipesHead
        {
            s_SubTextureVectorMap['Y'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 19.0f }, { 2.0f, 1.0f })); // Green
            s_SubTextureVectorMap['Y'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 15.0f }, { 2.0f, 1.0f })); // Orange
            s_SubTextureVectorMap['Y'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 13.0f }, { 2.0f, 1.0f })); // Grey
            s_SubTextureVectorMap['Y'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 9.0f }, { 2.0f, 1.0f }));  // Pink
            
            s_SubTextureVectorMap['!'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 18.0f }, { 2.0f, 1.0f })); // Green
            s_SubTextureVectorMap['!'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 14.0f }, { 2.0f, 1.0f })); // Orange
            s_SubTextureVectorMap['!'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 12.0f }, { 2.0f, 1.0f })); // Grey
            s_SubTextureVectorMap['!'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 8.0f }, { 2.0f, 1.0f }));  // Pink
            
            s_TextureMap['Y'] = s_SubTextureVectorMap['Y'][3];
            s_TextureMap['!'] = s_SubTextureVectorMap['!'][3];
        }
        
        // Adding Texture maps
        // Ground
        
        // Castel
        s_TextureMap['.'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 19.0f, 25.0f });
        s_TextureMap['u'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 20.0f, 24.0f });
        s_TextureMap['o'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 21.0f, 24.0f });
        s_TextureMap['|'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 21.0f, 25.0f });
        s_TextureMap['l'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 20.0f, 25.0f });
        s_TextureMap['r'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 22.0f, 25.0f });
                        
        for (uint32_t y = 0; y < mapHeight; y++)
        {
            for (uint32_t x = 0; x < mapWidth; x++)
            {
                if (char tileType = s_MapTiles[x + y * mapWidth];
                    s_TextureMap.find(tileType) != s_TextureMap.end())
                {
                    std::shared_ptr<SubTexture> subTexture = s_TextureMap[tileType];
                    
                    auto entity = s_EntityVector[tileType].emplace_back(scene->CreateEntity(GetEntityNameFromChar(tileType)));
                    auto spriteEntity = entity.AddComponent<SpriteRendererComponent>(subTexture);
                    auto spriteSize   = spriteEntity.SubTexComp->GetSpriteSize();
                    
                    entity.GetComponent<TransformComponent>().SetTransform({ x, (mapHeight / 2.0f) - y }, { spriteSize.x, spriteSize.y });
                }
            }
        }
    }
    
    void Background::ImGuiRender()
    {
        if (ImGui::CollapsingHeader("BackGround"))
        {
            if (ImGui::TreeNode("Color"))
            {
                ImGuiAPI::ColorEdit(s_BgColor);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Tiles"))
            {
                ImgButtons('G');
                ImgButtons('X');
                ImgButtons('B');
                ImgButtons('S');
                ImgButtons('-');
                ImgButtons('Y');
                ImgButtons('!');

                ImGui::TreePop();
            }
        }
    }
    
}
