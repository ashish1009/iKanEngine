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
        bool OnKeyPressed(KeyPressedEvent& e);
        
        void NewScene();
        void OpenScene();
        void SaveSceneAs();
        
    private:
        int m_GizmoType = -1;

        bool m_ViewportFocused = false, m_ViewportHovered = false;
        
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        
        Ref<Framebuffer> m_FrameBuffer;
        Ref<Scene>       m_ActiveScene;
        
        SceneHeirarchyPannel m_SceneHierarchyPannel;
        EditorCamera         m_EditorCamera;
        
        glm::vec4 m_BgColor = { 0.05f, 0.05f, 0.05f, 1.0f };
    };
    
}
