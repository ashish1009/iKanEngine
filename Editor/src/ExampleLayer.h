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
        bool OnMouseScroll(MouseScrollEvent& event);
        
    private:
        std::shared_ptr<VertexArray> m_VAO, m_LightVAO;
        std::shared_ptr<Shader> m_Shader, m_LightShader;
//        uint32_t m_VAO, m_LightVAO;
        uint32_t m_WhiteTextureId, m_CheckBoardTextureID, m_GridTextureID, m_GrassTextureID;        
    };
    
}