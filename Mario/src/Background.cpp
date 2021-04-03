// ******************************************************************************
//   File    : Background.cpp
//   Project : Mario
//
//   Created by Ashish
// ******************************************************************************

#include "Background.h"

namespace Mario {

    // ******************************************************************************
    // Scene String. each char represnet a entity described below
    // ******************************************************************************
    /********************************************************************************
     0 : END OF LINE TO GET NUMBER OF ELEMENT IN A ROW
     G : Ground
     | : Castel Brick
     o : Castel Gate
     u : castel Gate Domb
     . : Castel Domb
     l : Castel Windlow Left
     r : Castel Window Right
     S : Steps
     - : Bridge
     ! : Pipe Base
     Y : Pipe Head
     X : Bricks
     B : Bonus
     ( : Cloud Left
     ^ : Cloud
     ) : Cloud Right
     < : Grass Left
     v : Grass
     > : Grass Right
     ********************************************************************************/
    // TODO: For now making number of row even to make position of y integral
    static std::string s_MapTiles =
    "                                                                                                                                                                                                                                                                                                                            0"
    "                                                                                                                                                                                                                                                                                                                            0"
    "                                  (^)                                                                  (^)                                                                                           (^^^)                                                                         (^^)                                     0"
    "       (^)                                              (^^^)                                                                                  (^^)                                                                                           (^)                                                                  .        0"
    "                                                                                                                                                                                                                                                                                                                  ...       0"
    "                                                                                                                                                                                                                                                                                                                  |u|       0"
    "                                                                                                                                                                                                                                                                                                                  |o|       0"
    "                                                                                                                                                                                                                                                                                                                .......     0"
    "                                               B                  XXXX                             B            XBX                                                                 B                                                                 B                                                         |u|r|u|     0"
    "                                                                                                                                                                                                                                                                                                                |o|||o|     0"
    "                                                                                                                                                                                                                                                                                                              ...........   0"
    "                                                                                                                                                                                                                                                                                                              |||||||||||   0"
    "                                                                                                                                                                                                                                                                                                              |l|u|r|u|l|   0"
    "                                             XBXBX              XBXXXXBX                           S           XXXXX               B                                           B    B    B                                                          XBXBX                                                     |||o|||o|||   0"
    "                                                                                Y                 SS                                                                                                                          S  S                                                       S                  ............... 0"
    "                 *                                                              !                SSS                                                   S                                                 Y                   SS  SS                                                     SS                  ||||||||||||||| 0"
    "                {1}                                       Y                     !               SSSS                               Y                  SS        *                                        !                  SSS  SSS       *          Y                                SSS                  |u||u||u||u||u| 0"
    "  <v>          {123}                       <v>      S     !       S  S          !     <vv>     SSSSS                    <vvv>      !          <v>    SSS       {1}                                       !                 SSSS  SSSS     {1}         !         <v>                   SSSS         <v>      |o||o||o||o||0| 0"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG--------------------GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGG--------------------------GGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG0"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG                    GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGG                          GGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG  GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG0"
    ;

    // ******************************************************************************
    // Get entity name from Char code
    // ******************************************************************************
    static std::string GetEntityNameFromChar(char type)
    {
        switch(type)
        {
            case 'G' : return "Ground"; break;
                
            case '|' : return "Castel Brick";        break;
            case 'o' : return "Castel Gate";         break;
            case 'u' : return "castel Gate Domb";    break;
            case '.' : return "Castel Domb";         break;
            case 'l' : return "Castel Windlow Left"; break;
            case 'r' : return "Castel Window Right"; break;
                
            case 'S' : return "Steps";     break;
            case '-' : return "Bridge";    break;
            case '!' : return "Pipe Base"; break;
            case 'Y' : return "Pipe";      break;
            case 'X' : return "Bricks";    break;
            case 'B' : return "Bonus";     break;
                
            case '<' : return "Grass <"; break;
            case 'v' : return "Grass";   break;
            case '>' : return "Grass >"; break;
                
            case '(' : return "Cloud ("; break;
            case '^' : return "Cloud";   break;
            case ')' : return "Cloud )"; break;

            case '}' : return "Grass ";       break;
            case '{' : return "Grass Forest"; break;
            case '*' : return "Grass *";      break;
            case '1' : return "Grass 1";      break;
            case '2' : return "Grass 2";      break;
            case '3' : return "Grass 3";      break;
        }
        IK_ASSERT(false, "Invalid Type");
        return "";
    }
    
    // ******************************************************************************
    // Chnage the subtexture for each tiles in vector
    // ******************************************************************************
    static void ChangeEntitiesSubtex(const std::vector<Entity>& entityVector, const Ref<SubTexture>& subTex)
    {
        for (auto entity : entityVector)
            if (auto &subTexComp = entity.GetComponent<SpriteRendererComponent>().SubTexComp)
                subTexComp = subTex;
    }
    
    glm::vec4    BackgroundTile::s_BgColor         = { 0.3f, 0.1f, 0.6f, 1.0f };
    Ref<Texture> BackgroundTile::s_TileSpriteSheet = nullptr;

    std::unordered_map<char, BackgroundTile::Tile>          BackgroundTile::s_TextureMap;
    std::unordered_map<char, std::vector<Entity>>           BackgroundTile::s_EntityVector;
    std::unordered_map<char, std::vector<Ref<SubTexture>>>  BackgroundTile::s_SubTextureVectorMap;

    // ******************************************************************************
    // Initialize the background tiles
    // ******************************************************************************
    void BackgroundTile::Init()
    {
        IK_INFO("Initializing the Background tiles");
        // Ground SubTexutres
        {
            s_SubTextureVectorMap['G'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 27.0f })); // Brown
            s_SubTextureVectorMap['G'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 25.0f })); // Blue
            s_SubTextureVectorMap['G'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 23.0f })); // Grey
            s_SubTextureVectorMap['G'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 21.0f })); // Green

            s_TextureMap['G'].SubTexture = s_SubTextureVectorMap['G'][0];
        }

        // Bricks SubTextures
        {
            s_SubTextureVectorMap['X'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 17.0f, 27.0f })); // Brown;
            s_SubTextureVectorMap['X'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 17.0f, 25.0f })); // Blue;
            s_SubTextureVectorMap['X'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 17.0f, 23.0f })); // Grey;
            s_SubTextureVectorMap['X'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 17.0f, 21.0f })); // green;

            s_TextureMap['X'].SubTexture = s_SubTextureVectorMap['X'][0];
        }

        // Bonus SubTextures
        {
            s_SubTextureVectorMap['B'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 24.0f, 27.0f })); // Brown
            s_SubTextureVectorMap['B'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 24.0f, 25.0f })); // Blue
            s_SubTextureVectorMap['B'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 24.0f, 23.0f })); // Grey
            s_SubTextureVectorMap['B'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 24.0f, 21.0f })); // Green

            s_SubTextureVectorMap['B'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 27.0f, 27.0f })); // Brown
            s_SubTextureVectorMap['B'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 27.0f, 25.0f })); // Blue
            s_SubTextureVectorMap['B'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 27.0f, 23.0f })); // Grey
            s_SubTextureVectorMap['B'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 27.0f, 21.0f })); // Green

            s_TextureMap['B'].SubTexture = s_SubTextureVectorMap['B'][0];
        }

        // Steps SubTextures
        {
            s_SubTextureVectorMap['S'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 26.0f })); // Brown
            s_SubTextureVectorMap['S'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 24.0f })); // Blue
            s_SubTextureVectorMap['S'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 22.0f })); // Grey
            s_SubTextureVectorMap['S'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 20.0f })); // Green

            s_TextureMap['S'].SubTexture = s_SubTextureVectorMap['S'][0];
        }

        // Bridge SubTexture
        {
            s_SubTextureVectorMap['-'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 15.0f, 18.5f })); // Green
            s_SubTextureVectorMap['-'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 15.0f, 14.5f })); // Orange
            s_SubTextureVectorMap['-'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 15.0f, 10.5f })); // Grey
            s_SubTextureVectorMap['-'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 15.0f,  8.5f })); // Pink

            s_TextureMap['-'].SubTexture = s_SubTextureVectorMap['-'][0];
        }

        // PipesHead
        {
            s_SubTextureVectorMap['Y'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 19.0f }, { 2.0f, 1.0f })); // Green
            s_SubTextureVectorMap['Y'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 15.0f }, { 2.0f, 1.0f })); // Orange
            s_SubTextureVectorMap['Y'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 13.0f }, { 2.0f, 1.0f })); // Grey
            s_SubTextureVectorMap['Y'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f,  9.0f }, { 2.0f, 1.0f }));  // Pink

            s_SubTextureVectorMap['!'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 18.0f }, { 2.0f, 1.0f })); // Green
            s_SubTextureVectorMap['!'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 14.0f }, { 2.0f, 1.0f })); // Orange
            s_SubTextureVectorMap['!'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 12.0f }, { 2.0f, 1.0f })); // Grey
            s_SubTextureVectorMap['!'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f,  8.0f }, { 2.0f, 1.0f }));  // Pink

            s_TextureMap['Y'].SubTexture = s_SubTextureVectorMap['Y'][0];
            s_TextureMap['!'].SubTexture = s_SubTextureVectorMap['!'][0];
        }

        // Grass
        {
            s_SubTextureVectorMap['<'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 11.0f, 18.0f })); // Green
            s_SubTextureVectorMap['<'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 11.0f, 14.0f })); // Orange
            s_SubTextureVectorMap['<'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 11.0f, 12.0f })); // Grey
            s_SubTextureVectorMap['<'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 11.0f,  8.0f })); // Pink

            s_TextureMap['<'].SubTexture = s_SubTextureVectorMap['<'][0];
            s_TextureMap['<'].IsRigid = false;

            s_SubTextureVectorMap['v'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 12.0f, 18.0f })); // Green
            s_SubTextureVectorMap['v'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 12.0f, 14.0f })); // Orange
            s_SubTextureVectorMap['v'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 12.0f, 12.0f })); // Grey
            s_SubTextureVectorMap['v'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 12.0f,  8.0f })); // Pink

            s_TextureMap['v'].SubTexture = s_SubTextureVectorMap['v'][0];
            s_TextureMap['v'].IsRigid = false;

            s_SubTextureVectorMap['>'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 13.0f, 18.0f })); // Green
            s_SubTextureVectorMap['>'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 13.0f, 14.0f })); // Orange
            s_SubTextureVectorMap['>'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 13.0f, 12.0f })); // Grey
            s_SubTextureVectorMap['>'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 13.0f, 8.0f  })); // Pink

            s_TextureMap['>'].SubTexture = s_SubTextureVectorMap['>'][0];
            s_TextureMap['>'].IsRigid = false;
        }

        // Forest Grass
        {
            s_SubTextureVectorMap['{'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 8.0f, 19.0f })); // Green
            s_SubTextureVectorMap['{'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 8.0f, 15.0f })); // Orange
            s_SubTextureVectorMap['{'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 8.0f, 13.0f })); // Grey
            s_SubTextureVectorMap['{'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 8.0f,  9.0f })); // Pink

            s_TextureMap['{'].SubTexture = s_SubTextureVectorMap['{'][0];
            s_TextureMap['{'].IsRigid = false;

            s_SubTextureVectorMap['}'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 10.0f, 19.0f })); // Green
            s_SubTextureVectorMap['}'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 10.0f, 15.0f })); // Orange
            s_SubTextureVectorMap['}'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 10.0f, 13.0f })); // Grey
            s_SubTextureVectorMap['}'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 10.0f,  9.0f })); // Pink

            s_TextureMap['}'].SubTexture = s_SubTextureVectorMap['}'][0];
            s_TextureMap['}'].IsRigid = false;


            s_SubTextureVectorMap['*'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 9.0f, 19.0f })); // Green
            s_SubTextureVectorMap['*'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 9.0f, 15.0f })); // Orange
            s_SubTextureVectorMap['*'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 9.0f, 13.0f })); // Grey
            s_SubTextureVectorMap['*'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 9.0f,  9.0f })); // Pink

            s_TextureMap['*'].SubTexture = s_SubTextureVectorMap['*'][0];
            s_TextureMap['*'].IsRigid = false;

            s_SubTextureVectorMap['1'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 8.0f, 18.0f })); // Green
            s_SubTextureVectorMap['1'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 8.0f, 14.0f })); // Orange
            s_SubTextureVectorMap['1'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 8.0f, 12.0f })); // Grey
            s_SubTextureVectorMap['1'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 8.0f,  8.0f })); // Pink

            s_TextureMap['1'].SubTexture = s_SubTextureVectorMap['1'][0];
            s_TextureMap['1'].IsRigid = false;

            s_SubTextureVectorMap['2'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 9.0f, 18.0f })); // Green
            s_SubTextureVectorMap['2'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 9.0f, 14.0f })); // Orange
            s_SubTextureVectorMap['2'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 9.0f, 12.0f })); // Grey
            s_SubTextureVectorMap['2'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 9.0f,  8.0f })); // Pink

            s_TextureMap['2'].SubTexture = s_SubTextureVectorMap['2'][0];
            s_TextureMap['2'].IsRigid = false;

            s_SubTextureVectorMap['3'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 10.0f, 18.0f })); // Green
            s_SubTextureVectorMap['3'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 10.0f, 14.0f })); // Orange
            s_SubTextureVectorMap['3'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 10.0f, 12.0f })); // Grey
            s_SubTextureVectorMap['3'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 10.0f,  8.0f })); // Pink

            s_TextureMap['3'].SubTexture = s_SubTextureVectorMap['3'][0];
            s_TextureMap['3'].IsRigid = false;
        }

        // Cloud
        {
            s_SubTextureVectorMap['('].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 0.0f }, { 1.0f, 2.0f })); // Red
            s_SubTextureVectorMap['('].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 2.0f }, { 1.0f, 2.0f })); // Red
            s_SubTextureVectorMap['('].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 0.0f }, { 1.0f, 2.0f })); // Grey
            s_SubTextureVectorMap['('].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, 2.0f }, { 1.0f, 2.0f })); // Blue

            s_TextureMap['('].SubTexture = s_SubTextureVectorMap['('][0];
            s_TextureMap['('].IsRigid = false;

            s_SubTextureVectorMap['^'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 1.0f, 0.0f }, { 1.0f, 2.0f })); // Red
            s_SubTextureVectorMap['^'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 1.0f, 2.0f }, { 1.0f, 2.0f })); // Red
            s_SubTextureVectorMap['^'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 1.0f, 0.0f }, { 1.0f, 2.0f })); // Grey
            s_SubTextureVectorMap['^'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 1.0f, 2.0f }, { 1.0f, 2.0f })); // Blue

            s_TextureMap['^'].SubTexture = s_SubTextureVectorMap['^'][0];
            s_TextureMap['^'].IsRigid = false;

            s_SubTextureVectorMap[')'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 2.0f, 0.0f }, { 1.0f, 2.0f })); // Red
            s_SubTextureVectorMap[')'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 2.0f, 2.0f }, { 1.0f, 2.0f })); // Red
            s_SubTextureVectorMap[')'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 2.0f, 0.0f }, { 1.0f, 2.0f })); // Grey
            s_SubTextureVectorMap[')'].emplace_back(SubTexture::CreateFromCoords(s_TileSpriteSheet, { 2.0f, 2.0f }, { 1.0f, 2.0f })); // Blue

            s_TextureMap[')'].SubTexture = s_SubTextureVectorMap[')'][0];
            s_TextureMap[')'].IsRigid = false;
        }

        // Castel
        s_TextureMap['.'].SubTexture = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 19.0f, 25.0f });
        s_TextureMap['.'].IsRigid = false;

        s_TextureMap['u'].SubTexture = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 20.0f, 24.0f });
        s_TextureMap['u'].IsRigid = false;

        s_TextureMap['o'].SubTexture = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 21.0f, 24.0f });
        s_TextureMap['o'].IsRigid = false;

        s_TextureMap['|'].SubTexture = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 21.0f, 25.0f });
        s_TextureMap['|'].IsRigid = false;

        s_TextureMap['l'].SubTexture = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 20.0f, 25.0f });
        s_TextureMap['l'].IsRigid = false;

        s_TextureMap['r'].SubTexture = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 22.0f, 25.0f });
        s_TextureMap['r'].IsRigid = false;
    }
    
    // ******************************************************************************
    // Initialize the background tiles and creat entities
    // ******************************************************************************
    void BackgroundTile::CreateEntities(Ref<Scene>& scene)
    {
        // Texture tile
        s_TileSpriteSheet = Texture::Create("../../Mario/assets/Resources/Graphics/Tile.png");
        
        // Initialise the tiles and map
        Init();

        // Extract the map widht. MAP Width should be same for each New line string
        auto mapWidth = s_MapTiles.find_first_of('0') + 1;
        uint32_t mapHeight = static_cast<uint32_t>(strlen(s_MapTiles.c_str())) / mapWidth;
        
        // Creating entity for each tiles
        IK_INFO("Creating Entity for each tiles");
        for (uint32_t y = 0; y < mapHeight; y++)
        {
            for (uint32_t x = 0; x < mapWidth; x++)
            {
                if (char tileType = s_MapTiles[x + y * mapWidth]; s_TextureMap.find(tileType) != s_TextureMap.end())
                {
                    auto entity = s_EntityVector[tileType].emplace_back(scene->CreateEntityWithID(UUID(), GetEntityNameFromChar(tileType)));

                    if (s_TextureMap[tileType].IsRigid)
                    {
                        entity.AddComponent<BoxCollider2DComponent>();
                    }
                    
                    entity.GetComponent<SceneHierarchyPannelProp>().IsProp = false;// s_TextureMap[tileType].IsProp;

                    auto spriteEntity = entity.AddComponent<SpriteRendererComponent>(s_TextureMap[tileType].SubTexture);
                    auto spriteSize   = spriteEntity.SubTexComp->GetSpriteSize();

                    auto& tc = entity.GetComponent<TransformComponent>();

                    tc.Translation = { x, (mapHeight / 2.0f) - y, 0.0f };
                    tc.Scale       = { spriteSize.x, spriteSize.y , 0.0f};
                } //if (char tileType = s_MapTiles[x + y * mapWidth]; s_TextureMap.find(tileType) != s_TextureMap.end())
            } // for (uint32_t x = 0; x < mapWidth; x++)
        } // for (uint32_t y = 0; y < mapHeight; y++)
    }

    // ******************************************************************************
    // Imgui interface for chnaging tiles image using buttons
    // ******************************************************************************
    void BackgroundTile::ImgButtons(const char name)
    {
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
                {
                    ChangeEntitiesSubtex(s_EntityVector[name], subTex);

                    if (name == 'Y')
                    {
                        ChangeEntitiesSubtex(s_EntityVector['!'], s_SubTextureVectorMap['!'][i]);
                    }

                    else if (name == 'v')
                    {
                        ChangeEntitiesSubtex(s_EntityVector['<'], s_SubTextureVectorMap['<'][i]);
                        ChangeEntitiesSubtex(s_EntityVector['>'], s_SubTextureVectorMap['>'][i]);
                    }

                    else if (name == '^')
                    {
                        ChangeEntitiesSubtex(s_EntityVector[')'], s_SubTextureVectorMap[')'][i]);
                        ChangeEntitiesSubtex(s_EntityVector['('], s_SubTextureVectorMap['('][i]);
                    }

                    else if (name == '{')
                    {
                        ChangeEntitiesSubtex(s_EntityVector['*'], s_SubTextureVectorMap['*'][i]);
                        ChangeEntitiesSubtex(s_EntityVector['}'], s_SubTextureVectorMap['}'][i]);
                        ChangeEntitiesSubtex(s_EntityVector['1'], s_SubTextureVectorMap['1'][i]);
                        ChangeEntitiesSubtex(s_EntityVector['2'], s_SubTextureVectorMap['2'][i]);
                        ChangeEntitiesSubtex(s_EntityVector['3'], s_SubTextureVectorMap['3'][i]);
                    }
                }
                ImGui::PopID();
                ImGui::SameLine();
                i++;
            }
            ImGui::NewLine();
            ImGui::TreePop();
        }
    }

    // ******************************************************************************
    // Imgui update each frame for background
    // ******************************************************************************
    void BackgroundTile::ImGuiRenderer()
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
            ImgButtons('v');
            ImgButtons('^');
            ImgButtons('{');

            ImGui::TreePop();
        }

    }

}


