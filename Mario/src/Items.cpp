#include "Items.h"

namespace Mario {

    Ref<Scene> Items::m_Scene = nullptr;
    Ref<Texture> Items::s_TileSpriteSheet = nullptr;
    std::unordered_map<std::string, Ref<SubTexture>> Items::s_ItemMap;
    std::vector<Entity> Items::s_CoinEntity;

    // New Mario Specific Component
    void Items::Init(Ref<Scene>& scene)
    {
        m_Scene = scene;

        // Texture tile
        s_TileSpriteSheet = Texture::Create("../../Mario/assets/Resources/Graphics/Items.png");

        s_ItemMap["Coin"]       = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, Type::Coin });
        s_ItemMap["SideCoin"]   = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, Type::SideCoin });
        s_ItemMap["WinFlag"]    = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, Type::WinFlag });
        s_ItemMap["Star"]       = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, Type::Star });
        s_ItemMap["Fire"]       = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, Type::Fire });
        s_ItemMap["Mushroom"]   = SubTexture::CreateFromCoords(s_TileSpriteSheet, { 0.0f, Type::Mushroom });
    }

    void Items::Onpdate(TimeStep ts)
    {
        static float counter = 0;
        for (auto entity : s_CoinEntity)
        {
            auto& posision = entity.GetComponent<TransformComponent>().Translation;
            posision.y += 0.1f;

            auto& subTex = entity.GetComponent<SpriteRendererComponent>().SubTexComp;
            if ((int32_t)counter % 2)
            {
                subTex = s_ItemMap["Coin"];
            }
            else
            {
                subTex = s_ItemMap["SideCoin"];
            }
        }

        if (s_CoinEntity.size() > 0)
        {
            counter += (ts * 7);
        }
    }

    void Items::CreateBonusItem(Entity colloidedEntity)
    {
        Entity entity = s_CoinEntity.emplace_back(m_Scene->CreateEntityWithID(UUID(), "Coin"));
        entity.AddComponent<SpriteRendererComponent>(s_ItemMap["Coin"]);

        entity.AddComponent<BoxCollider2DComponent>();

        auto& itemPos = entity.GetComponent<TransformComponent>().Translation;
        const auto& colloidedEntityPos = colloidedEntity.GetComponent<TransformComponent>().Translation;
        itemPos.x = colloidedEntityPos.x;
        itemPos.y = colloidedEntityPos.y + 1;
    }

}



