#include <iKan/Renderer/FrameBuffer.h>
#include <iKan/Renderer/RenderAPI.h>

#include <iKan/Platform/OpenGL/OpenGLFrameBuffer.h>

namespace iKan {
    
    std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
    {
        switch (RenderAPI::GetAPI())
        {
            case API::None:    IK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case API::OpenGL:  return std::make_shared<OpenGLFrameBuffer>(spec);
        }
        
        IK_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
    
}
