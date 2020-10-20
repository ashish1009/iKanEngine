#include <iKan/Renderer/API/RendererCommand.h>

namespace iKan {
    
    Scope<RendererAPI> RendererCommand::s_RenderAPI = RendererAPI::Create();
    
}
