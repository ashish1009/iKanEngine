#include <iKan/Renderer/SceneRenderer.h>
#include <iKan/Renderer/Renderer.h>
#include <iKan/Renderer/Shader.h>
#include <iKan/Renderer/VertexArray.h>
#include <iKan/Renderer/Buffers.h>
#include <iKan/Renderer/Texture.h>
#include <iKan/Renderer/RenderStats.h>

namespace iKan {
    
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        
        float     TexIndex;
        float     TilingFactor;
    };
    
    struct Renderer2DData
    {
        // Consts to store limits of renderer
        // TODO: work on MAX
        static const uint32_t MaxQuads        = 20000;
        static const uint32_t MaxVertices     = MaxQuads * 8;
        static const uint32_t MaxIndices      = MaxQuads * 36;
        static const uint32_t MaxTextureSlots = 16;
        
        // Data storage for Rendering
        Ref<VertexArray>    QuadVertexArray;
        Ref<VertexBuffer>   QuadVertexBuffer;
        Ref<Shader>         TextureShader;
        Ref<Texture>        WhiteTexture;
        
        uint32_t    QuadIndexCount = 0;
        
        // Pointer attribute of vertexes
        QuadVertex* QuadVertexBufferBase    = nullptr;
        QuadVertex* QuadVertexBufferPtr     = nullptr;
        
        // array of textures for now 16 slots are possible
        std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; /* 0 = white texture */
        
        // Basic vertex of quad
        glm::vec4 QuadVertexPositions[8];
    };
    
    static Renderer2DData s_Data;
    
    void SceneRenderer::Init()
    {
        IK_CORE_INFO("Initialising Renderer2D ");
        s_Data.QuadVertexArray  = VertexArray::Create();
        
        // Vertex Buffer and adding the layput
        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
        s_Data.QuadVertexBuffer->AddLayout ({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float,  "a_TexIndex" },
            { ShaderDataType::Float,  "a_TilingFactor" }
        });
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
        
        // Allocating the memory for vertex Buffer Pointer
        s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
        
        // Index Buffer
        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
        
        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MaxIndices; i += 36)
        {
            quadIndices[i + 0] = offset + 0;        quadIndices[i + 6] = offset + 1;        quadIndices[i + 12] = offset + 7;
            quadIndices[i + 1] = offset + 1;        quadIndices[i + 7] = offset + 5;        quadIndices[i + 13] = offset + 6;
            quadIndices[i + 2] = offset + 2;        quadIndices[i + 8] = offset + 6;        quadIndices[i + 14] = offset + 5;
            quadIndices[i + 3] = offset + 2;        quadIndices[i + 9] = offset + 6;        quadIndices[i + 15] = offset + 5;
            quadIndices[i + 4] = offset + 3;        quadIndices[i + 10] = offset + 2;        quadIndices[i + 16] = offset + 4;
            quadIndices[i + 5] = offset + 0;        quadIndices[i + 11] = offset + 1;        quadIndices[i + 17] = offset + 7;
            
            quadIndices[i + 18] = offset + 4;        quadIndices[i + 24] = offset + 4;        quadIndices[i + 30] = offset + 3;
            quadIndices[i + 19] = offset + 0;        quadIndices[i + 25] = offset + 5;        quadIndices[i + 31] = offset + 2;
            quadIndices[i + 20] = offset + 3;        quadIndices[i + 26] = offset + 1;        quadIndices[i + 32] = offset + 6;
            quadIndices[i + 21] = offset + 3;        quadIndices[i + 27] = offset + 1;        quadIndices[i + 33] = offset + 6;
            quadIndices[i + 22] = offset + 7;        quadIndices[i + 28] = offset + 0;        quadIndices[i + 34] = offset + 7;
            quadIndices[i + 23] = offset + 0;        quadIndices[i + 29] = offset + 4;        quadIndices[i + 35] = offset + 3;
            
            offset += 8;
        }
        
        Ref<IndexBuffer> quadIB = IndexBuffer::Create(s_Data.MaxIndices, quadIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
        delete[] quadIndices;
        
        // Creating whote texture for cololful quads witout any texture or sprite
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture       = Texture::Create(1, 1, &whiteTextureData, sizeof(uint32_t));
        
        // Set the Texture slot 0 as White Texture
        s_Data.TextureSlots[0] = s_Data.WhiteTexture;
        
        // Setting basic Vertex point of quad
        s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.5f, 1.0f };
        s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.5f, 1.0f };
        s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.5f, 1.0f };
        s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.5f, 1.0f };
        
        s_Data.QuadVertexPositions[4] = { -0.5f, -0.5f, -0.5f, 1.0f };
        s_Data.QuadVertexPositions[5] = {  0.5f, -0.5f, -0.5f, 1.0f };
        s_Data.QuadVertexPositions[6] = {  0.5f,  0.5f, -0.5f, 1.0f };
        s_Data.QuadVertexPositions[7] = { -0.5f,  0.5f, -0.5f, 1.0f };

    }
    
    void SceneRenderer::SetShaader(const std::string &path)
    {
        // Creating array of Slots to store hem in shader
        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
            samplers[i] = i;
        
        // Creating Shader and storing all the slots
        s_Data.TextureShader = Shader::Create(path);
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
    }
    
    void SceneRenderer::Shutdown()
    {
        delete[] s_Data.QuadVertexBufferBase;
    }
    
    void SceneRenderer::BeginScene(const Camera& camera, const glm::mat4& transform)
    {
        // Upload Camera View Projection Matris to shader
        glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);
        
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetUniformMat4("u_ViewProjection", viewProj);
        
        // initialise the Vertex Array Attrib pointer adn Index count
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        
        // Initialise the Texture Slot to 1 as 0 is set for white textute
        s_Data.TextureSlotIndex = 1;
    }
    
    void SceneRenderer::EndScene()
    {
        // getting the Number of vertex but subtracting the running and base pointers
        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
        
        Flush();
    }
    
    void SceneRenderer::Flush()
    {
        // Nothing to draw
        if (s_Data.QuadIndexCount == 0)
            return;
        
        // Bind textures
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
            s_Data.TextureSlots[i]->Bind(i);
        
        // Render the Scene
        Renderer::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
        RendererStatistics::DrawCalls++;
    }
    
    void SceneRenderer::FlushAndReset()
    {
        // if num Quad per Batch exceeds then Render the Scene and reset all parameters
        EndScene();
        
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        
        s_Data.TextureSlotIndex = 1;
    }
    
    /* -------------------------- Premitives -------------------------------------------------*/
    
    void SceneRenderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
    {
        constexpr size_t    quadVertexCount = 8;
        const float         textureIndex    = 0.0f; // White Texture
        // TODO: FIX
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        const float         tilingFactor    = 1.0f;
        
        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
            FlushAndReset();
        
        for (size_t i = 0; i < quadVertexCount; i++)
        {
            s_Data.QuadVertexBufferPtr->Position        = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color           = color;
            s_Data.QuadVertexBufferPtr->TexCoord        = textureCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex        = textureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor    = tilingFactor;
            s_Data.QuadVertexBufferPtr++;
        }
        
        s_Data.QuadIndexCount += 66;
        
        RendererStatistics::QuadCount++;
        
    }
    
}
