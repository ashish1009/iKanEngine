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
    };
    
}
