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
        
    };
    
}
