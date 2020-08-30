#pragma once

#include <iKan/Renderer/RenderAPI.h>

namespace iKan {

    class OpenGlRenderAPI : public RenderAPI
    {
    public:
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
        virtual void Depth(State state) override;
        virtual void Blend(State state) override;
        virtual void SetViewPort(uint32_t widht, uint32_t height) override;
    };
    
}
