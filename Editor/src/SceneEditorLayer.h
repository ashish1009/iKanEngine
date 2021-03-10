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
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
        
        void NewScene();
        void OpenScene();
        void SaveSceneAs();
        
    private:
        struct Viewport
        {
            bool Focused = false;
            bool Hovered = false;
            
            float Width  = 0.0f;
            float Height = 0.0f;
            
            int32_t MouseX = -1;
            int32_t MouseY = -1;
    
            glm::vec2 Size = { 0.0f, 0.0f };
            glm::vec2 Bounds[2];
            
            Ref<Framebuffer> FrameBuffer;
        };
        Viewport m_Viewport;
        
        int32_t m_GizmoType = -1;
        
        Ref<Scene>           m_ActiveScene;
        Entity               m_HoveredEntity;
        SceneHeirarchyPannel m_SceneHierarchyPannel;
        EditorCamera         m_EditorCamera;
        
        glm::vec4 m_BgColor = { 0.05f, 0.05f, 0.05f, 1.0f };
    };
    
}
