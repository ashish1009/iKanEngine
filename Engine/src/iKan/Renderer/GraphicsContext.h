#pragma once

namespace iKan {
    
    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;
        
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        
        static std::unique_ptr<GraphicsContext> CreateContext(GLFWwindow* window);
    };
    
}
