#include <iKan.h>

namespace iKan {
    
    class SceneEditor : public Layer
    {
    public:
        SceneEditor();
        virtual ~SceneEditor();
        
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
        
        Entity m_CameraEntity;
        
        SceneHeirarchyPannel m_SceneHierarchyPannel;

    };
    
}
