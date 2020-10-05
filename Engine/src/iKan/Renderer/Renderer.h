#pragma once

#include <iKan/Renderer/RenderCommand.h>
#include <iKan/Renderer/Shader.h>

namespace iKan {
    
    class Renderer
    {
    public:
        static void Init();
        static void Clear(glm::vec4 color);
        static void Depth(State state) { RenderCommand::Depth(state); }
        static void Blend(State state) { RenderCommand::Blend(state); }
        static void SetViewport(float width, float height);
        static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0);
        static void DrawIndexed(uint32_t count);
        static void Shutdown();

        static Ref<ShaderLibrary> GetShaderLibrary();
    };
    
}
