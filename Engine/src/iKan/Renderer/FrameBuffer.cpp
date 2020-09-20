#include <iKan/Renderer/FrameBuffer.h>
#include <iKan/Renderer/RenderAPI.h>

#include <iKan/Platform/OpenGL/OpenGLFrameBuffer.h>

namespace iKan {
    
    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
    {
        switch (RenderAPI::GetAPI())
        {
            case API::None:    IK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case API::OpenGL:  return Ref<OpenGLFrameBuffer>::Create(spec);
        }
        
        IK_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
    
}
