#pragma once

#include <iKan/Renderer/Camera.h>

namespace iKan {
    
    class RenderCubeMap
    {
    public:
        static void Init();
        static void Shutdown();
        
        static void BeginScene(const Camera& camera, const glm::mat4& transform);

        static void EndScene();
        static void Flush();
        
        static void SetShaader(const std::string &cubeMapPath);
        static void SetCubeMapTexture(const std::vector<std::string>& paths);
        /* Primitives */
        static void DrawCube();
        
    private:
        static void FlushAndReset();
    };
    
}
