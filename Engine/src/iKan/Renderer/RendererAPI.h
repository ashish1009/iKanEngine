#pragma once

namespace iKan {
    
    enum class API { None = 0, OpenGL = 1 };
    
    class RendererAPI
    {
    public:
        static API GetAPI() { return s_API; } 
        
    private:
        static API s_API;
    };
    
}
