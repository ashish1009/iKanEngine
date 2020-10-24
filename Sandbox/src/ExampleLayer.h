#include <iKan.h>

namespace iKan {

    class ExampleLayer : public Layer
    {
    public:
        ExampleLayer();
        virtual ~ExampleLayer() = default;
        
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& event) override;
        virtual void OnUpdate(TimeStep timeStep) override;
        virtual void OnImguiRender() override;
        
    private:
        bool OnWindowResize(WindowResizeEvent& event);
        bool OnMouseScroll(MouseScrolledEvent& event);
        
    private:
        Ref<VertexArray> m_VAO, m_LightVAO;
        
        ShaderLibrary m_ShaderLibrary;
        
        Ref<Texture> m_WhiteTexture, m_CheckboardTexture, m_GridTexture, m_GrassTexture;
    };
    
}
