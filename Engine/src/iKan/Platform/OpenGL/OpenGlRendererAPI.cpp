#include <iKan/Platform/OpenGL/OpenGlRendererAPI.h>

#include <glad/glad.h>

namespace iKan {
    
    void OpenGlRendererAPI::Init()
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        auto& caps = RendererAPI::GetCapabilities();
        
        caps.Vendor   = (const char*)glGetString(GL_VENDOR);
        caps.Renderer = (const char*)glGetString(GL_RENDERER);
        caps.Version  = (const char*)glGetString(GL_VERSION);
    }
    
    void OpenGlRendererAPI::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }
    
    void OpenGlRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    void OpenGlRendererAPI::Depth(State state)
    {
        if (State::Enable == state)
            glEnable(GL_DEPTH_TEST);
        else if (State::Disable == state)
            glDisable(GL_DEPTH_TEST);
    }
    
    void OpenGlRendererAPI::Blend(State state)
    {
        if (State::Enable == state)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else if (State::Disable == state)
        {
            glDisable(GL_BLEND);
        }
    }
    
    void OpenGlRendererAPI::SetViewPort(uint32_t widht, uint32_t height)
    {
        glViewport(0, 0, widht, height);
    }
    
    void OpenGlRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count)
    {
        vertexArray->Bind();
        uint32_t numIndices = count == 0 ? vertexArray->GetIndexBuffer()->GetCount() : count;
        
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        vertexArray->Unbind();
    }

    void OpenGlRendererAPI::DrawIndexed(uint32_t count)
    {
        glDrawArrays(GL_TRIANGLES, 0, count);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}
