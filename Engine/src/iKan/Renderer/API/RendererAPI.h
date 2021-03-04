// ******************************************************************************
//   File    : RendererAPI.h
//   Project : i-Kan : Renderer
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <iKan/Renderer/Graphics/VertexArray.h>

namespace iKan {
    
    enum class API { None = 0, OpenGL = 1 };
    enum class State { Disable = 0, Enable = 1 };
    
    // ******************************************************************************
    // Stores the capability of renderer
    // ******************************************************************************
    struct RenderAPICapabilities
    {
        std::string Vendor;
        std::string Renderer;
        std::string Version;
    };
    
    // ******************************************************************************
    // Stores the renderer API type, or graphics type and call the renderer function
    // accordingly
    // ******************************************************************************
    class RendererAPI
    {
    public:
        virtual ~RendererAPI() = default;

        static API GetAPI() { return s_API; }
        static Scope<RendererAPI> Create();
        
        virtual void Init() = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;
        virtual void Depth(State state) = 0;
        virtual void Blend(State state) = 0;
        virtual void SetViewPort(uint32_t widht, uint32_t height) = 0;
        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0) = 0;
        virtual void DrawIndexed(uint32_t count) = 0;
        
        static RenderAPICapabilities& GetCapabilities()
        {
            static RenderAPICapabilities capabilities;
            return capabilities;
        }
        
    private:
        static API s_API;
    };
    
}
