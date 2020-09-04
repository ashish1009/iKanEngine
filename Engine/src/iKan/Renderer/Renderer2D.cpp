#include <iKan/Renderer/Renderer2D.h>

#include <iKan/Renderer/RenderCommand.h>
#include <iKan/Renderer/VertexArray.h>
#include <iKan/Renderer/Buffers.h>
#include <iKan/Renderer/Shader.h>
#include <iKan/Renderer/Texture.h>

namespace iKan {
    
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoords;
        
        float TextureIndex;
        float TilingFactor;
    };
    
    struct RendererData
    {
        static const uint32_t MaxTextureSlots   = 16;
        static const uint32_t MaxNumQuad        = 10000;
        static const uint32_t MaxVertices       = MaxNumQuad * NumQuadVertex;
        static const uint32_t MaxIndices        = MaxNumQuad * NumQuadIndices;
        
        std::shared_ptr<VertexArray>  QuadVertexArray;
        std::shared_ptr<VertexBuffer> QuadVertexBuffer;
        std::shared_ptr<IndexBuffer>  QuadIndexBuffer;
        std::shared_ptr<Texture>      WhiteTexture;
        std::shared_ptr<Shader>       Shader;
        
        uint32_t QuadIndexCount = 0;
        
        QuadVertex* QuadVertexPtr     = nullptr;
        QuadVertex* QuadVertexBasePtr = nullptr;

        glm::vec4 QuadVertexPositions[4];
        
        Renderer2D::Statistics Stats;
    };
    static RendererData s_Data;
    
    void Renderer2D::Init()
    {
        s_Data.QuadVertexArray  = VertexArray::Create();
        
        s_Data.QuadVertexBuffer = VertexBuffer::Create(RendererData::MaxVertices);
        s_Data.QuadVertexBuffer->AddLayout({
            { ShaderDataType::Float3, "s_Position" },
            { ShaderDataType::Float4, "s_Color" },
            { ShaderDataType::Float2, "s_TexCoord" },
            { ShaderDataType::Float,  "s_TextureIndex" },
            { ShaderDataType::Float,  "s_TilingFactor" }
        });
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
        
        uint32_t* quadIndices = new uint32_t[RendererData::MaxIndices];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < RendererData::MaxIndices; i += NumQuadIndices)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;
            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;
            offset += NumQuadVertex;
        }
        
        s_Data.QuadIndexBuffer = IndexBuffer::Create(RendererData::MaxIndices, quadIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(s_Data.QuadIndexBuffer);
        delete[] quadIndices;
        
        s_Data.QuadVertexBasePtr = new QuadVertex[RendererData::MaxNumQuad];
        
        s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
        
        uint32_t whiteTextureData = 0xffffffff;
        void* whiteTextureDataPtr = &whiteTextureData;
        s_Data.WhiteTexture = Texture::Create(1, 1, whiteTextureDataPtr, sizeof(uint32_t));
    }
    
    void Renderer2D::AddShader(const std::string& path)
    {
        int32_t samplers[RendererData::MaxTextureSlots];
        for (uint32_t i = 0; i < RendererData::MaxTextureSlots; i++)
            samplers[i] = i;
        
        s_Data.Shader = Shader::Create(path);
        s_Data.Shader->Bind();
        s_Data.Shader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
    }
    
    void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
    {
        glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);
        
        s_Data.Shader->Bind();
        s_Data.Shader->SetUniformMat4("u_ViewProjection", viewProj);
        s_Data.QuadVertexPtr = s_Data.QuadVertexBasePtr;

        s_Data.QuadIndexCount = 0;
    }
    
    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
    {
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        
        for (uint32_t i = 0; i < NumQuadVertex ; i++)
        {
            s_Data.QuadVertexPtr->Position      = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexPtr->Color         = color;
            s_Data.QuadVertexPtr->TexCoords     = textureCoords[i];
            s_Data.QuadVertexPtr->TextureIndex  = 0.0f; // White Texture
            s_Data.QuadVertexPtr->TilingFactor  = 1.0f;
            s_Data.QuadVertexPtr++;
        }
        s_Data.QuadIndexCount += NumQuadIndices;
        
        s_Data.Stats.QuadCount++;
    }
    
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        
        DrawQuad(transform, color);
    }
    
    void Renderer2D::EndScene()
    {
        uint32_t size = (uint32_t)((uint8_t*)s_Data.QuadVertexPtr - (uint8_t*)s_Data.QuadVertexBasePtr);
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBasePtr, size);
        
        s_Data.WhiteTexture->Bind();
        
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
        s_Data.Stats.DrawCalls++;
    }
    
    void Renderer2D::Shutdown()
    {
        delete[] s_Data.QuadVertexBasePtr;
    }
    
    void Renderer2D::ImguiStatsAnfFrameRate()
    {
        //------------------------ Statistics -------------------------------------------------------------
        ImGui::Begin("Stats");
        const auto& stats = GetStats();
        ImGui::Text("Draw Calls : %d, Quad Counts : %d, VertexCounts : %d, Index Counts : %d", stats.DrawCalls, stats.QuadCount, stats.GetTotalVertexCount(), stats.GetTotalIndexCount());
        ImGui::End();
        
        //------------------------ Frame Rates -------------------------------------------------------------
        ImGui::Begin("Frame Rate");
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
    
    // --------------------------------- Statistics -------------------------------------------------------
    
    void Renderer2D::ResetStats()
    {
        memset(&s_Data.Stats, 0, sizeof(Statistics));
    }
    
    Renderer2D::Statistics Renderer2D::GetStats()
    {
        return s_Data.Stats;
    }

}
