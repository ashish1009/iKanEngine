#include <iKan.h>

#include "Player.h"

namespace iKan {
    
    class MarioLayer : public Layer
    {
    public:
        MarioLayer();
        virtual ~MarioLayer();
        
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& event) override;
        virtual void OnUpdate(TimeStep timeStep) override;
        virtual void OnImguiRender() override;
        
    private:
        bool m_ViewportFocused = false, m_ViewportHovered = false;

        std::shared_ptr<Framebuffer> m_FrameBuffer;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        
        std::shared_ptr<Texture> m_TileSpriteSheet;
        std::shared_ptr<Texture> m_PlayerSpriteSheet;
        
        std::shared_ptr<SubTexture> m_PlayerSubtexture;
        
        std::unordered_map<char, std::shared_ptr<SubTexture>> m_TextureMap;
        uint32_t m_MapWidth = 0, m_MapHeight = 0;

        std::shared_ptr<Scene> m_Scene;
        
        Entity m_CameraEntity;
        Entity m_PlayerEntity;
        
        Player* m_PlayerInstance;
            
    };
    
}
