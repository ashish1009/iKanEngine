#include <Layers/Background.h>
#include <Layers/SceneMap.h>

namespace iKan {

    static std::shared_ptr<Texture> s_TileSpriteSheet;
    static std::unordered_map<char, std::shared_ptr<SubTexture>> s_TextureMap;
    
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
    
    void Background::Init(const std::shared_ptr<Scene>& scene)
    {
        // Texture tile
        s_TileSpriteSheet = Texture::Create("../../Mario/assets/Resources/Graphics/Tile.png");
        
        uint32_t mapWidth          = s_MapWidth;
        uint32_t mapHeight         = static_cast<uint32_t>(strlen(s_MapTiles)) / s_MapWidth;
        
        // Adding Texture maps
        // Ground
        s_TextureMap['G'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 21.0f });
        
        // Castel
        s_TextureMap['.'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 19.0f, 25.0f });
        s_TextureMap['u'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 20.0f, 24.0f });
        s_TextureMap['o'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 21.0f, 24.0f });
        s_TextureMap['|'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 21.0f, 25.0f });
        s_TextureMap['l'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 20.0f, 25.0f });
        s_TextureMap['r'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 22.0f, 25.0f });
        
        // Steps
        s_TextureMap['S'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 22.0f });
        
        // Bridge
        s_TextureMap['-'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 15.0f, 19.0f });
        
        // Pipes
        s_TextureMap['!'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 8.0f }, { 2.0f, 1.0f });
        s_TextureMap['Y'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 9.0f }, { 2.0f, 1.0f });
        
        // Bonus and Bricks
        s_TextureMap['X'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 17.0f, 25.0f });;
        s_TextureMap['B'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 24.0f, 21.0f });
        
        for (uint32_t y = 0; y < mapHeight; y++)
        {
            for (uint32_t x = 0; x < mapWidth; x++)
            {
                if (char tileType = s_MapTiles[x + y * mapWidth];
                    s_TextureMap.find(tileType) != s_TextureMap.end())
                {
                    std::shared_ptr<SubTexture> subTexture = s_TextureMap[tileType];
                    
                    Entity entity     = scene->CreateEntity(GetEntityNameFromChar(tileType));
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
            if (ImGui::TreeNode("Color"))
                ImGuiAPI::ColorEdit(s_BgColor);
    }
    
}
