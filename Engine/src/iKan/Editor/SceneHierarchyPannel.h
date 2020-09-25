#pragma once

#include <iKan.h>

namespace iKan {
    
    class SceneHeirarchyPannel
    {
    public:
        SceneHeirarchyPannel() = default;
        SceneHeirarchyPannel(const Ref<Scene>& scene);
        
        void SetContext(const Ref<Scene>& scene);
        
        void OnImguiender();
        
    private:
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);

    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };
    
}
