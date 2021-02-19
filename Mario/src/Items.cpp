#include "Items.h"

namespace Mario {

    Ref<Scene> Items::m_Scene = nullptr;
    Ref<Texture> Items::s_TileSpriteSheet = nullptr;
    std::unordered_map<std::string, Ref<SubTexture>> Items::s_ItemMap;
    std::vector<ItemEntity> Items::s_CoinEntity;

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
        for (auto it = s_CoinEntity.begin(); it != s_CoinEntity.end(); it++)
        {
            auto& entity = it->Entity;
            auto& posision = entity.GetComponent<TransformComponent>().Translation;
            if (posision.y < it->yEndPosition)
            {
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
            else
            {
                m_Scene->DestroyEntity(entity);
                s_CoinEntity.erase(it);
                break;
            }
        }

        if (s_CoinEntity.size() > 0)
        {
            counter += (ts * 7);
        }
    }

    void Items::CreateBonusItem(Entity colloidedEntity)
    {
        const auto& colloidedEntityPos = colloidedEntity.GetComponent<TransformComponent>().Translation;

        auto& coinEntity = s_CoinEntity.emplace_back(ItemEntity(m_Scene->CreateEntityWithID(UUID(), "Coin"), colloidedEntityPos.y + 5));
        coinEntity.Entity.AddComponent<SpriteRendererComponent>(s_ItemMap["Coin"]);

        coinEntity.Entity.AddComponent<BoxCollider2DComponent>();

        auto& itemPos = coinEntity.Entity.GetComponent<TransformComponent>().Translation;
        itemPos.x = colloidedEntityPos.x;
        itemPos.y = colloidedEntityPos.y + 1;
    }

}



