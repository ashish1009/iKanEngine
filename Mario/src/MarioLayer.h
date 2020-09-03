#include <iKan.h>

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
        std::shared_ptr<Framebuffer> m_Framebuffer;
        
        bool m_ViewportFocused = false, m_ViewportHovered = false;
        
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

    };
    
}
