#pragma once

#include <iKan/Renderer/RenderAPI.h>

namespace iKan {

    class RenderCommand
    {
    public:
        static void SetClearColor(const glm::vec4& color) { s_RenderAPI->SetClearColor(color); }
        static void Clear() { s_RenderAPI->Clear(); }
        static void Depth(State state) { s_RenderAPI->Depth(state); }
        static void Blend(State state) { s_RenderAPI->Blend(state); }
        static void SetViewPort(uint32_t widht, uint32_t height) { s_RenderAPI->SetViewPort(widht, height); }
        static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count = 0) { s_RenderAPI->DrawIndexed(vertexArray, count); }
        
    private:
        static Scope<RenderAPI> s_RenderAPI;
    };
    
}
