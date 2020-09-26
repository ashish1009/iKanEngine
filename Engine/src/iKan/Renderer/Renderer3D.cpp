#include <iKan/Renderer/Renderer3D.h>
#include <iKan/Renderer/Renderer.h>
#include <iKan/Renderer/Shader.h>
#include <iKan/Renderer/VertexArray.h>
#include <iKan/Renderer/Buffers.h>
#include <iKan/Renderer/Texture.h>
#include <iKan/Renderer/RenderStats.h>

namespace iKan {
    
    static const uint32_t NumCubeVertices = 8;
    static const uint32_t NumCubeIndices  = 36;
    
    struct CubeVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        
        float     TexIndex;
        float     TilingFactor;
    };
    
    struct Renderer3DData
    {
        // Consts to store limits of renderer
        static const uint32_t MaxCubes        = 20000;
        static const uint32_t MaxVertices     = MaxCubes * NumCubeVertices;
        static const uint32_t MaxIndices      = MaxCubes * NumCubeIndices;
        static const uint32_t MaxTextureSlots = 16;
        
        // Data storage for Rendering
        Ref<VertexArray>    CubeVertexArray;
        Ref<VertexBuffer>   CubeVertexBuffer;
        Ref<Shader>         TextureShader;
        Ref<Texture>        WhiteTexture;
        
        uint32_t    CubeIndexCount = 0;
        
        // Pointer attribute of vertexes
        CubeVertex* CubeVertexBufferBase    = nullptr;
        CubeVertex* CubeVertexBufferPtr     = nullptr;
        
        // array of textures for now 16 slots are possible
        std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; /* 0 = white texture */
        
        // Basic vertex of quad
        glm::vec4 CubeVertexPositions[8];
    };
    static Renderer3DData s_Data;
    
    void Renderer3D::Init()
    {
        IK_CORE_INFO("Initialising Renderer2D ");
        s_Data.CubeVertexArray  = VertexArray::Create();
        
        // Vertex Buffer and adding the layput
        s_Data.CubeVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CubeVertex));
        s_Data.CubeVertexBuffer->AddLayout ({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float,  "a_TexIndex" },
            { ShaderDataType::Float,  "a_TilingFactor" }
        });
        s_Data.CubeVertexArray->AddVertexBuffer(s_Data.CubeVertexBuffer);
        
        // Allocating the memory for vertex Buffer Pointer
        s_Data.CubeVertexBufferBase = new CubeVertex[s_Data.MaxVertices];
        
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
        s_Data.CubeVertexArray->SetIndexBuffer(quadIB);
        delete[] quadIndices;
        
        // Creating whote texture for cololful quads witout any texture or sprite
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture       = Texture::Create(1, 1, &whiteTextureData, sizeof(uint32_t));
        
        // Set the Texture slot 0 as White Texture
        s_Data.TextureSlots[0] = s_Data.WhiteTexture;
        
        // Setting basic Vertex point of quad
        s_Data.CubeVertexPositions[0] = { -0.5f, -0.5f, 0.5f, 1.0f };
        s_Data.CubeVertexPositions[1] = {  0.5f, -0.5f, 0.5f, 1.0f };
        s_Data.CubeVertexPositions[2] = {  0.5f,  0.5f, 0.5f, 1.0f };
        s_Data.CubeVertexPositions[3] = { -0.5f,  0.5f, 0.5f, 1.0f };
        
        s_Data.CubeVertexPositions[4] = { -0.5f, -0.5f, -0.5f, 1.0f };
        s_Data.CubeVertexPositions[5] = {  0.5f, -0.5f, -0.5f, 1.0f };
        s_Data.CubeVertexPositions[6] = {  0.5f,  0.5f, -0.5f, 1.0f };
        s_Data.CubeVertexPositions[7] = { -0.5f,  0.5f, -0.5f, 1.0f };
        
    }
    
    void Renderer3D::SetShaader(const std::string &path)
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
    
    void Renderer3D::Shutdown()
    {
        delete[] s_Data.CubeVertexBufferBase;
    }
    
    void Renderer3D::BeginScene(const Camera& camera, const glm::mat4& transform)
    {
        // Upload Camera View Projection Matris to shader
        glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);
        
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetUniformMat4("u_ViewProjection", viewProj);
        
        // initialise the Vertex Array Attrib pointer adn Index count
        s_Data.CubeIndexCount = 0;
        s_Data.CubeVertexBufferPtr = s_Data.CubeVertexBufferBase;
        
        // Initialise the Texture Slot to 1 as 0 is set for white textute
        s_Data.TextureSlotIndex = 1;
    }
    
    void Renderer3D::EndScene()
    {
        // getting the Number of vertex but subtracting the running and base pointers
        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CubeVertexBufferPtr - (uint8_t*)s_Data.CubeVertexBufferBase);
        s_Data.CubeVertexBuffer->SetData(s_Data.CubeVertexBufferBase, dataSize);
        
        Flush();
    }
    
    void Renderer3D::Flush()
    {
        // Nothing to draw
        if (s_Data.CubeIndexCount == 0)
            return;
        
        // Bind textures
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
            s_Data.TextureSlots[i]->Bind(i);
        
        // Render the Scene
        Renderer::DrawIndexed(s_Data.CubeVertexArray, s_Data.CubeIndexCount);
        RendererStatistics::DrawCalls++;
    }
    
    void Renderer3D::FlushAndReset()
    {
        // if num Cube per Batch exceeds then Render the Scene and reset all parameters
        EndScene();
        
        s_Data.CubeIndexCount = 0;
        s_Data.CubeVertexBufferPtr = s_Data.CubeVertexBufferBase;
        
        s_Data.TextureSlotIndex = 1;
    }
    
    /* -------------------------- Premitives -------------------------------------------------*/
    
    void Renderer3D::DrawCube(const glm::mat4& transform, const glm::vec4& color)
    {
        const float         textureIndex    = 0.0f; // White Texture
                                                    // TODO: FIX
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        const float         tilingFactor    = 1.0f;
        
        if (s_Data.CubeIndexCount >= Renderer3DData::MaxIndices)
            FlushAndReset();
        
        for (size_t i = 0; i < NumCubeIndices; i++)
        {
            s_Data.CubeVertexBufferPtr->Position        = transform * s_Data.CubeVertexPositions[i];
            s_Data.CubeVertexBufferPtr->Color           = color;
            s_Data.CubeVertexBufferPtr->TexCoord        = textureCoords[i];
            s_Data.CubeVertexBufferPtr->TexIndex        = textureIndex;
            s_Data.CubeVertexBufferPtr->TilingFactor    = tilingFactor;
            s_Data.CubeVertexBufferPtr++;
        }
        
        s_Data.CubeIndexCount += NumCubeIndices;
        
        // TODO: Fix
        RendererStatistics::QuadCount += NumCubeVertices;
        
    }
    
}
