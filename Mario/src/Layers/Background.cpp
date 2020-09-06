#include "Background.h"
#include "SceneMap.h"

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
        s_TextureMap['G'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0, 21 });
        
        // Castel
        s_TextureMap['.'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 19, 25 });
        s_TextureMap['u'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 20, 24 });
        s_TextureMap['o'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 21, 24 });
        s_TextureMap['|'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 21, 25 });
        s_TextureMap['l'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 20, 25 });
        s_TextureMap['r'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 22, 25 });
        
        // Steps
        s_TextureMap['S'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0, 22 });
        
        // Bridge
        s_TextureMap['-'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 15, 19 });
        
        // Pipes
        s_TextureMap['!'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0, 8 }, { 2, 1 });
        s_TextureMap['Y'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0, 9 }, { 2, 1 });
        
        // Bonus and Bricks
        s_TextureMap['X'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 17, 25 });;
        s_TextureMap['B'] = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 24,  21 });
        
        for (uint32_t y = 0; y < mapHeight; y++)
        {
            for (uint32_t x = 0; x < mapWidth; x++)
            {
                char tileType = s_MapTiles[x + y * mapWidth];
                if (s_TextureMap.find(tileType) != s_TextureMap.end())
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
        {
            if (ImGui::TreeNode("Color"))
            {
                static ImVec4 color         = ImVec4(s_BgColor.r, s_BgColor.g, s_BgColor.b, s_BgColor.a);
                static ImVec4 refColorValue = color;
                
                static bool alphaPreview = true, alphaHalfPreview = true;
                ImGui::Checkbox("Alpha", &alphaPreview);  ImGui::SameLine(); ImGui::Checkbox("Half Alpha", &alphaHalfPreview);
                ImGuiColorEditFlags miscFlags = ImGuiColorEditFlags_PickerHueWheel | (alphaHalfPreview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alphaPreview ? ImGuiColorEditFlags_AlphaPreview : 0));
                if (alphaPreview || alphaHalfPreview) miscFlags |= ImGuiColorEditFlags_AlphaBar; else miscFlags |= ImGuiColorEditFlags_NoAlpha;
                
                static bool sidePreview = true, refColor = false;
                ImGui::Checkbox("Side Preview", &sidePreview);
                if (sidePreview)
                {
                    ImGui::SameLine();
                    ImGui::Checkbox("Ref Color", &refColor);
                    if (refColor)
                    {
                        ImGui::SameLine();
                        ImGui::ColorEdit4("##RefColor", &refColorValue.x, ImGuiColorEditFlags_NoInputs | miscFlags);
                    }
                }
                if (!sidePreview)
                    miscFlags |= ImGuiColorEditFlags_NoSidePreview;
                
                ImGui::ColorPicker4("Back Ground##4", (float*)&color, miscFlags, refColor ? &refColorValue.x : NULL);
                ImGui::TreePop();
                
                s_BgColor = { color.x, color.y, color.z, color.w };
            }
        }
    }
    
}
