#pragma once

namespace iKan {
    
    class Texture
    {
    public:
        virtual ~Texture() = default;
        
        virtual void Bind(uint32_t slot = 0) const = 0;
        virtual void Unbind() const = 0;
        
        static std::shared_ptr<Texture> Create(const std::string& path);
        static std::shared_ptr<Texture> Create(uint32_t width, uint32_t height, void* data, uint32_t size);
    };
    
}
