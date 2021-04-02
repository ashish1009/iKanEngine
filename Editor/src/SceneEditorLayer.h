// ******************************************************************************
//   File    : SceneEditor.h
//   Project : Editor
//
//   Created by Ashish
// ******************************************************************************

#include <iKan.h>

namespace iKan {
    
    // ******************************************************************************
    // Editor class which is client layer
    // ******************************************************************************
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
        Entity               m_HoveredEntity;
        SceneHeirarchyPannel m_SceneHierarchyPannel;
        EditorCamera         m_EditorCamera;
        
        glm::vec4 m_BgColor = { 0.05f, 0.05f, 0.05f, 1.0f };

        Viewport& m_Viewport = Viewport::Get();
    };
    
}
