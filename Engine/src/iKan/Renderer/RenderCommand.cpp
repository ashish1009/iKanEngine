#include <iKan/Renderer/RenderCommand.h>

namespace iKan {
    
    std::unique_ptr<RenderAPI> RenderCommand::s_RenderAPI = RenderAPI::Create();
    
}
