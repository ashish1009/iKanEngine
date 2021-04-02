#include <iKan.h>

using namespace iKan;

namespace Mario {
    
    class MarioLayer : public Layer
    {
    public:
        MarioLayer();
        virtual ~MarioLayer();
        
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& event) override;
        virtual void OnUpdate(TimeStep timeStep) override;
        virtual void OnImguiRender() override;
        
    private:
        bool OnKeyPressed(KeyPressedEvent& event);
        bool OnKeyReleased(KeyReleasedEvent& event);
        
        static void ShowMenue();
        static void RendererStats();
        
    private:
        Ref<Scene>           m_ActiveScene;
        SceneHeirarchyPannel m_SceneHierarchyPannel;
        
        Viewport& m_Viewport = Viewport::Get();
        
        struct PropertyFlag
        {
            bool IsFrameRate           = true;
            bool IsRendererStats       = true;
            bool IsVendorType          = true;
            bool IsSetting             = false;
            bool IsSceneHeirarchypanel = false;
        };
        static PropertyFlag s_PropFlag;
        
    };
    
}
