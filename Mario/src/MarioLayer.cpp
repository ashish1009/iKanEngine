#include "MarioLayer.h"

namespace iKan {
    
    MarioLayer::MarioLayer()
    {
        
    }

    MarioLayer::~MarioLayer()
    {
        
    }

    void MarioLayer::OnAttach()
    {
        
    }
    
    void MarioLayer::OnDetach()
    {
        
    }
    
    void MarioLayer::OnEvent(Event& event)
    {
        
    }
                             
    void MarioLayer::OnUpdate(TimeStep timeStep)
    {
        RenderCommand::Clear();
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        
        Renderer2D::BeginScene("../../Mario/assets/shaders/Shader.glsl");
        
        Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.3f, 0.5f, 0.7f, 1.0f });
        
        Renderer2D::EndScene();
    }
                              
    void MarioLayer::OnImguiRender()
    {
        
    }
                                   
}
