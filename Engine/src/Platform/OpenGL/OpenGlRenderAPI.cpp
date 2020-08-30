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
        
    }
    
    void OpenGlRenderAPI::SetViewPort(uint32_t widht, uint32_t height)
    {
        glViewport(0, 0, widht, height);
    }
    
}
