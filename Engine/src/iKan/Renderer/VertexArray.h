#pragma once

namespace iKan {
    
    class VertexArray
    {
    public:
        virtual ~VertexArray() = default;
        
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        
        static std::shared_ptr<VertexArray> Create();
    };
    
}
