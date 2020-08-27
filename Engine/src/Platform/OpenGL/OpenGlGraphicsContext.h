#pragma once

#include <iKan/Renderer/GraphicsContext.h>

namespace iKan {
    
    class OpneGlGraphicsContext : public GraphicsContext
    {
    public:
        OpneGlGraphicsContext(GLFWwindow* window);
        virtual ~OpneGlGraphicsContext() = default;
        
        virtual void Init();
        virtual void SwapBuffers();
        
    private:
        GLFWwindow* m_Window;
    };
    
}
