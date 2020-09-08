#include <Layers/Player.h>

#include <NativeScripts/PlayerMove.h>

namespace iKan {

    Entity Player::s_Entity;
    float Player::s_Color = static_cast<float>(PlayerColor::BlueOrange);
    std::shared_ptr<SubTexture> Player::s_StandSubtexture = nullptr;
    std::shared_ptr<Texture> Player::s_SpriteSheet = nullptr;
    
    void Player::Init(const std::shared_ptr<Scene>& scene)
    {
        IK_INFO("Player Instance Created");
        
        s_SpriteSheet     = Texture::Create("../../Mario/assets/Resources/Graphics/Player.png");
        s_StandSubtexture = SubTexture::CreateFromCoords(s_SpriteSheet, { 6.0f, s_Color });
                
        s_Entity = scene->CreateEntity("Player");
        s_Entity.AddComponent<SpriteRendererComponent>(s_StandSubtexture);
                
        s_Entity.AddComponent<NativeScriptComponent>().Bind<PlayerMove>();
    }
        
}
