#include <iKan.h>

using namespace iKan;

namespace Mario {
    
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
        bool OnKeyPressed(KeyPressedEvent& event);
        bool OnKeyReleased(KeyReleasedEvent& event);
        
    private:
        bool m_ViewportFocused = false, m_ViewportHovered = false;

        Ref<Framebuffer> m_FrameBuffer;
        Ref<Scene> m_ActiveScene;

        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        
        SceneHeirarchyPannel m_SceneHierarchyPannel;
    };
    
}
