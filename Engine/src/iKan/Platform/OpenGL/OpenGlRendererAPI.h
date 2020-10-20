#pragma once

#include <iKan/Renderer/API/RendererAPI.h>

namespace iKan {

    class OpenGlRendererAPI : public RendererAPI
    {
    public:
        virtual void Init() override;
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
        virtual void Depth(State state) override;
        virtual void Blend(State state) override;
        virtual void SetViewPort(uint32_t widht, uint32_t height) override;
        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0) override;
        virtual void DrawIndexed(uint32_t count) override;
    };
    
}
