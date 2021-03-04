// ******************************************************************************
//   File    : Renderer2D.h
//   Project : i-Kan : Renderer
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <iKan/Renderer/Camera/Camera.h>
#include <iKan/Renderer/Graphics/Texture.h>

#include <iKan/Editor/EditorCamera.h>

namespace iKan {
    
    // ******************************************************************************
    // Renderer class to draw 2d Using batch rendererr
    // ******************************************************************************
    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();
        
        static void BeginScene(const Camera& camera, const glm::mat4& transform);
        static void BeginScene(const EditorCamera& camera);

        static void EndScene();
        static void Flush();
        
        static void SetShaader(const std::string& path);
        
        /* Primitives */
        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int32_t entID);
        static void DrawQuad(const glm::mat4& transform, const Ref<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, int32_t entID = 0);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int32_t entID = 0);
        
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color, int32_t entID = 0);
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color, int32_t entID = 0);
        
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
                
    private:
        static void StartBatch();
        static void NextBatch();
        
    };
    
}
