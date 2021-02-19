#pragma once

#include <iKan.h>

using namespace iKan;

namespace Mario {

    struct ItemEntity
    {
        iKan::Entity Entity;
        float yEndPosition;

        ItemEntity(const iKan::Entity& entity, float yPos)
        : Entity(entity), yEndPosition(yPos)
        {

        }
    };

    class Items
    {
    public:
        enum class Type
        {
            WinFlag = 12, SideCoin = 13, Coin = 14, Star = 17, Fire = 18, Mushroom = 19
        };

    public:
        static void Init(Ref<Scene>& scene);
        static void CreateBonusItem(Entity colloidedEntity);

        static void Onpdate(TimeStep ts);

    private:
        static Ref<Texture> s_TileSpriteSheet;
        static std::unordered_map<std::string, Ref<SubTexture>> s_ItemMap;

        static std::vector<ItemEntity> s_CoinEntity;

        static Ref<Scene> m_Scene;
    };

}
