#include <iKan.h>

namespace iKan {
    
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
        Ref<Framebuffer> m_Framebuffer;
        
        bool m_ViewportFocused = false, m_ViewportHovered = false;
        bool m_PrimaryCamera = true;
        
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        
        Ref<Scene> m_ActiveScene;
    
        // pannels
        SceneHeirarchyPannel m_SceneHierarchyPannel;
    };
    
}
