#pragma once

#include <GLFW/glfw3.h>

namespace iKan {
    
    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;
        
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        
        static Scope<GraphicsContext> CreateContext(GLFWwindow* window);
    };
    
}
