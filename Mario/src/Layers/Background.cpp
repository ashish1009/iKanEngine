#include <Layers/Background.h>
#include <Layers/SceneMap.h>

namespace iKan {

    static std::shared_ptr<Texture> s_TileSpriteSheet;
    static std::unordered_map<char, std::shared_ptr<SubTexture>> s_TextureMap;
    static std::unordered_map<char, std::vector<Entity>> s_EntityVector;
    
    static std::unordered_map<std::string, std::shared_ptr<SubTexture>> s_GroundSubTextures;
    static std::unordered_map<std::string, std::shared_ptr<SubTexture>> s_BrickesSubTextures;
    static std::unordered_map<std::string, std::shared_ptr<SubTexture>> s_BonusSubTextures;
    static std::unordered_map<std::string, std::shared_ptr<SubTexture>> s_StepsSubTexture;
    static std::unordered_map<std::string, std::shared_ptr<SubTexture>> s_PipesHeadSubTextures;
    static std::unordered_map<std::string, std::shared_ptr<SubTexture>> s_PipesBaseSubTextures;
    static std::unordered_map<std::string, std::shared_ptr<SubTexture>> s_BridgeSubTextures;
    
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
    
    static std::unordered_map<std::string, std::shared_ptr<SubTexture>> GetSubTextureMap(const std::string& name)
    {
        std::unordered_map<std::string, std::shared_ptr<SubTexture>> nullResult;
        
        if (name == "Ground") return s_GroundSubTextures;
        else if (name == "Bricks") return s_BrickesSubTextures;
        else if (name == "Bonus") return s_BonusSubTextures;
        else if (name == "Steps") return s_StepsSubTexture;
        else if (name == "Pipes Head") return s_PipesHeadSubTextures;
        else if (name == "Pipes Base") return s_PipesBaseSubTextures;
        else if (name == "Bridge") return s_BridgeSubTextures;
        
        else IK_ASSERT(false, "Invalid name");
        
        return nullResult;
    }
    
    static std::vector<Entity> getEntityVector(const std::string& name)
    {
        std::vector<Entity> nullResult;
        
        if (name == "Ground") return s_EntityVector['G'];
        else if (name == "Bricks") return s_EntityVector['X'];
        else if (name == "Bonus") return s_EntityVector['B'];
        else if (name == "Steps") return s_EntityVector['S'];
        else if (name == "Pipes Head") return s_EntityVector['Y'];
        else if (name == "Pipes Base") return s_EntityVector['!'];
        else if (name == "Bridge") return s_EntityVector['-'];
        
        else IK_ASSERT(false, "Invalid name");
        
        return nullResult;
    }
    
    static void ImgButtons(const std::string& name)
    {
#pragma clang diagnostic ignored "-Wformat-security"
        if (ImGui::TreeNode(name.c_str()))
        {
            ImTextureID myTexId = (ImTextureID)((size_t)s_TileSpriteSheet->GetRendererID());
            float myTexW = (float)s_TileSpriteSheet->GetWidth();
            float myTexH = (float)s_TileSpriteSheet->GetHeight();
            
            int32_t i = 0;
            for (auto kv : GetSubTextureMap(name))
            {
                ImGui::PushID(i);
                
                auto coords = kv.second->GetCoords();
                glm::vec2 uv1 = { (coords.x + 1) * 16.0f, coords.y * 16.0f };
                glm::vec2 uv0 = { coords.x * 16.0f, (coords.y + 1) * 16.0f };
                if (ImGui::ImageButton(myTexId, ImVec2(32.0f, 32.0f), ImVec2(uv0.x / myTexW, uv0.y / myTexH), ImVec2(uv1.x / myTexW, uv1.y / myTexH), 0))
                    for (auto entity : getEntityVector(name))
                        if (auto &subTexComp = entity.GetComponent<SpriteRendererComponent>().SubTexComp)
                            subTexComp = kv.second;
                
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
            s_GroundSubTextures["Brown"] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 27.0f });
            s_GroundSubTextures["Blue"]  = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 25.0f });
            s_GroundSubTextures["Grey"]  = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 23.0f });
            s_GroundSubTextures["Green"] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 21.0f });
            
            s_TextureMap['G'] = s_GroundSubTextures["Green"];
        }
        
        // Bricks SubTextures
        {
            s_BrickesSubTextures["Brown"] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 17.0f, 27.0f });
            s_BrickesSubTextures["Blue"]  = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 17.0f, 25.0f });
            s_BrickesSubTextures["Grey"]  = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 17.0f, 23.0f });
            s_BrickesSubTextures["Green"] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 17.0f, 21.0f });
            
            s_TextureMap['X'] = s_BrickesSubTextures["Blue"];
        }
        
        // Bomus SubTextures
        {
            s_BonusSubTextures["Brown"] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 24.0f, 27.0f });
            s_BonusSubTextures["Blue"]  = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 24.0f, 25.0f });
            s_BonusSubTextures["Grey"]  = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 24.0f, 23.0f });
            s_BonusSubTextures["Green"] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 24.0f, 21.0f });
            
            s_TextureMap['B'] = s_BonusSubTextures["Green"];
        }
        
        // Steos SubTextures
        {
            s_StepsSubTexture["Brown"] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 26.0f });
            s_StepsSubTexture["Blue"]  = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 24.0f });
            s_StepsSubTexture["Grey"]  = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 22.0f });
            s_StepsSubTexture["Green"] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 20.0f });
            
            s_TextureMap['S'] = s_StepsSubTexture["Grey"];
        }
        
        // Bridge SubTexture
        {
            s_BridgeSubTextures["Green"]  = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 15.0f, 19.0f });
            s_BridgeSubTextures["Orange"] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 15.0f, 15.0f });
            s_BridgeSubTextures["Grey"]   = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 15.0f, 11.0f });
            s_BridgeSubTextures["Pink"]   = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 15.0f, 9.0f });
            
            s_TextureMap['-'] = s_BridgeSubTextures["Pink"];
        }
        
        // PipesHead
        {
            s_PipesHeadSubTextures["Green"]  = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 19.0f }, { 2.0f, 1.0f });
            s_PipesHeadSubTextures["Orange"] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 15.0f }, { 2.0f, 1.0f });
            s_PipesHeadSubTextures["Grey"]   = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 13.0f }, { 2.0f, 1.0f });
            s_PipesHeadSubTextures["Pink"]   = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 9.0f }, { 2.0f, 1.0f });
            
            s_PipesBaseSubTextures["Green"]  = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 18.0f }, { 2.0f, 1.0f });
            s_PipesBaseSubTextures["Orange"] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 14.0f }, { 2.0f, 1.0f });
            s_PipesBaseSubTextures["Grey"]   = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 12.0f }, { 2.0f, 1.0f });
            s_PipesBaseSubTextures["Pink"]   = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 8.0f }, { 2.0f, 1.0f });
            
            s_TextureMap['Y'] = s_PipesHeadSubTextures["Pink"];
            s_TextureMap['!'] = s_PipesBaseSubTextures["Pink"];
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
        ImGui::ShowDemoWindow();
        if (ImGui::CollapsingHeader("BackGround"))
        {
            if (ImGui::TreeNode("Color"))
            {
                ImGuiAPI::ColorEdit(s_BgColor);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Tiles"))
            {
                ImgButtons("Ground");
                ImgButtons("Bricks");
                ImgButtons("Bonus");
                ImgButtons("Steps");
                ImgButtons("Bridge");
                ImgButtons("Pipes Head");
                ImgButtons("Pipes Base");

                ImGui::TreePop();
            }
        }
    }
    
}
