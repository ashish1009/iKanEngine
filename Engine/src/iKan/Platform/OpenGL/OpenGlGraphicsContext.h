#pragma once

#include <iKan/Renderer/Graphics/GraphicsContext.h>

namespace iKan {
    
    class OpneGlGraphicsContext : public GraphicsContext
    {
    public:
        OpneGlGraphicsContext(GLFWwindow* window);
        virtual ~OpneGlGraphicsContext() = default;
        
        virtual void Init() override;
        virtual void SwapBuffers() override;
        
    private:
        GLFWwindow* m_Window;
    };
    
}
