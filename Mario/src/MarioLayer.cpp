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
        
        Renderer2D::ResetStats();
        
        Renderer2D::BeginScene("../../Mario/assets/shaders/Shader.glsl");
        
        Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.3f, 0.5f, 0.7f, 1.0f });
        
        Renderer2D::EndScene();
    }
                              
    void MarioLayer::OnImguiRender()
    {
        //------------------------ Frame Rates -------------------------------------------------------------
        ImGui::Begin("Frame Rate");
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        //------------------------ Statistics -------------------------------------------------------------
        ImGui::Begin("Stats");
        auto stats = Renderer2D::GetStats();
        ImGui::Text("Draw Calls   : %d", stats.DrawCalls);
        ImGui::Text("Quad Counts  : %d", stats.QuadCount);
        ImGui::Text("Vertex Count : %d", stats.GetTotalVertexCount());
        ImGui::Text("Index Cound  : %d", stats.GetTotalIndexCount());
        ImGui::End();
    }
                                   
}
