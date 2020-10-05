#include <iKan.h>

namespace iKan {
    
    static uint32_t s_EditorWidth  = 1600;
    static uint32_t s_EditorHeight = 800;
    
    class EntityLayer : public Layer
    {
    public:
        EntityLayer();
        virtual ~EntityLayer();
        
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& event) override;
        virtual void OnUpdate(TimeStep timeStep) override;
        virtual void OnImguiRender() override;
        
    private:
        bool m_ViewportFocused = false, m_ViewportHovered = false;

        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

        Ref<Framebuffer> m_FrameBuffer;
        Ref<Scene>       m_ActiveScene;
        
        SceneHeirarchyPannel m_SceneHierarchyPannel;
    };
    
}
