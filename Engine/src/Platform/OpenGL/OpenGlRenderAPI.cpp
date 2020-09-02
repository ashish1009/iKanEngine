#include <Platform/OpenGL/OpenGlRenderAPI.h>

namespace iKan {
    
    void OpenGlRenderAPI::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }
    
    void OpenGlRenderAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    void OpenGlRenderAPI::Depth(State state)
    {
        if (State::Enable == state)
            glEnable(GL_DEPTH_TEST);
        else if (State::Disable == state)
            glDisable(GL_DEPTH_TEST);
    }
    
    void OpenGlRenderAPI::Blend(State state)
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
    
    void OpenGlRenderAPI::SetViewPort(uint32_t widht, uint32_t height)
    {
        glViewport(0, 0, widht, height);
    }
    
    void OpenGlRenderAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count)
    {
        vertexArray->Bind();
        uint32_t numIndices = count == 0 ? vertexArray->GetIndexBuffer()->GetCount() : count;
        
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        vertexArray->Unbind();
    }
    
}
