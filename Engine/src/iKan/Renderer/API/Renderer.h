// ******************************************************************************
//   File    : Renderer.h
//   Project : i-Kan : Renderer
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <iKan/Renderer/API/RendererCommand.h>
#include <iKan/Renderer/Graphics/Shader.h>

namespace iKan {
    
    // ******************************************************************************
    // Wrapper renderer that is vissible to Application
    // ******************************************************************************
    class Renderer
    {
    public:
        static void Init();
        static void Clear(glm::vec4 color);
        static void Depth(State state) { RendererCommand::Depth(state); }
        static void Blend(State state) { RendererCommand::Blend(state); }
        static void SetViewport(float width, float height);
        static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0);
        static void DrawIndexed(uint32_t count);
        static void Shutdown();

        static Ref<ShaderLibrary> GetShaderLibrary();
    };
    
}
