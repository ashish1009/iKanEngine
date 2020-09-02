#include <iKan/Renderer/FrameBuffer.h>
#include <iKan/Renderer/Renderer.h>

#include <Platform/OpenGL/OpenGLFrameBuffer.h>

namespace iKan {
    
    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    IK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFrameBuffer>(spec);
        }
        
        IK_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
    
}
