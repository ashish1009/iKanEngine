#pragma once

#include <iKan.h>

namespace iKan {
    
    class SceneHeirarchyPannel
    {
    public:
        SceneHeirarchyPannel() = default;
        SceneHeirarchyPannel(const Ref<Scene>& scene);
        
        void SetContext(const Ref<Scene>& scene);
        void OnImguiender(bool* pIsOpen = nullptr);

        void SetSelectedEntity(Entity entity) { m_SelectionContext = entity; }
        Entity GetSelectedEntity() const { return m_SelectionContext; }
        
    private:
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);

    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };
    
}
