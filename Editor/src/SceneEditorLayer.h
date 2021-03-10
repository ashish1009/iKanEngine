#include "Viewport.h"

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
        
        void UpdateHoveredEntity();
        
        void ShowMenu();
        void PrintHoveredEntity();
        void UpdateGuizmo();
        
    private:
        
        int32_t m_GizmoType = -1;
        
        Ref<Scene>           m_ActiveScene;
        Viewport             m_Viewport;
        Entity               m_HoveredEntity;
        SceneHeirarchyPannel m_SceneHierarchyPannel;
        EditorCamera         m_EditorCamera;
        
        glm::vec4 m_BgColor = { 0.05f, 0.05f, 0.05f, 1.0f };
    };
    
}
