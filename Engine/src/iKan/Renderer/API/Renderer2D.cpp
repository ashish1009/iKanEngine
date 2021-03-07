// ******************************************************************************
//   File    : Renderer2D.cpp
//   Project : i-Kan : Renderer
//
//   Created by Ashish
// ******************************************************************************

#include <iKan/Renderer/API/Renderer2D.h>
#include <iKan/Renderer/API/Renderer.h>
#include <iKan/Renderer/API/RendererStats.h>

#include <iKan/Renderer/Graphics/VertexArray.h>
#include <iKan/Renderer/Graphics/Shader.h>

#include <iKan/Platform/OpenGL/OpenGlShader.h>

namespace iKan {
    
    // ******************************************************************************
    // Stores the vertex information of a Quad
    // ******************************************************************************
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;

        float TexIndex;
        float TilingFactor;

        int32_t ObjectID;
    };
    
    // ******************************************************************************
    // Stores the 2D Renderer information. All data and buffer pointers
    // ******************************************************************************
    struct Renderer2DData
    {
        // Consts to store limits of renderer
        static const uint32_t MaxQuads        = 20000;
        static const uint32_t MaxVertices     = MaxQuads * 4;
        static const uint32_t MaxIndices      = MaxQuads * 6;
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
        glm::vec4 QuadVertexPositions[4];

        Renderer2DData() = default;
        ~Renderer2DData()
        {
            IK_CORE_WARN("Renderer2D Data instance Destryoyed and cleared the data");
            delete[] QuadVertexBufferBase;
            delete[] QuadVertexBufferPtr;
        }
    };
    static Renderer2DData* s_Data = new Renderer2DData();
    
    // ******************************************************************************
    // Initialise the 2D renderer
    // ******************************************************************************
    void Renderer2D::Init()
    {
        IK_CORE_INFO("Initialising Renderer2D ");
        s_Data->QuadVertexArray  = VertexArray::Create();
        
        // Vertex Buffer and adding the layput
        s_Data->QuadVertexBuffer = VertexBuffer::Create(s_Data->MaxVertices * sizeof(QuadVertex));
        s_Data->QuadVertexBuffer->AddLayout ({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float,  "a_TexIndex" },
            { ShaderDataType::Float,  "a_TilingFactor" },
            { ShaderDataType::Int,    "a_ObjectID" }
        });
        s_Data->QuadVertexArray->AddVertexBuffer(s_Data->QuadVertexBuffer);
        
        // Allocating the memory for vertex Buffer Pointer
        s_Data->QuadVertexBufferBase = new QuadVertex[s_Data->MaxVertices];
        
        // Index Buffer
        uint32_t* quadIndices = new uint32_t[s_Data->MaxIndices];
        
        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data->MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;
            
            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;
            
            offset += 4;
        }
        
        Ref<IndexBuffer> quadIB = IndexBuffer::Create(s_Data->MaxIndices, quadIndices);
        s_Data->QuadVertexArray->SetIndexBuffer(quadIB);
        delete[] quadIndices;
        
        // Creating whote texture for cololful quads witout any texture or sprite
        uint32_t whiteTextureData = 0xffffffff;
        s_Data->WhiteTexture       = Texture::Create(1, 1, &whiteTextureData, sizeof(uint32_t));
                
        // Set the Texture slot 0 as White Texture
        s_Data->TextureSlots[0] = s_Data->WhiteTexture;
        
        // Setting basic Vertex point of quad
        s_Data->QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data->QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        s_Data->QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        s_Data->QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
        
        SetShaader("../../Engine/assets/shaders/BatchRenderer2DShader.glsl");
    }
    
    // ******************************************************************************
    // Set the 2DD shader
    // ******************************************************************************
    void Renderer2D::SetShaader(const std::string &path)
    {
        IK_CORE_INFO("Set the 2D Renderer Shader {0}", path.c_str());

        // Creating array of Slots to store hem in shader
        int32_t samplers[s_Data->MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data->MaxTextureSlots; i++)
            samplers[i] = i;
        
        // Creating Shader and storing all the slots
        s_Data->TextureShader = Shader::Create(path);
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetIntArray("u_Textures", samplers, s_Data->MaxTextureSlots);
    }
    
    // ******************************************************************************
    // Shut down the 2D Renderer
    // ******************************************************************************
    void Renderer2D::Shutdown()
    {
        IK_CORE_WARN("Shutting down the 2D renderer");
        delete s_Data;
    }
    
    // ******************************************************************************
    // Begin the 2D Scene
    // ******************************************************************************
    void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
    {
        // Upload Camera View Projection Matris to shader
        glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);
        
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetUniformMat4("u_ViewProjection", viewProj);
        
        StartBatch();
    }

    // ******************************************************************************
    // Begin the 2D Scene
    // ******************************************************************************
    void Renderer2D::BeginScene(const EditorCamera& camera)
    {
        glm::mat4 viewProj = camera.GetViewProjection();

        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetUniformMat4("u_ViewProjection", viewProj);

        StartBatch();
    }
    
    // ******************************************************************************
    // End the 2D Scene
    // ******************************************************************************
    void Renderer2D::EndScene()
    {
        Flush();
    }
    
    // ******************************************************************************
    // Start the batch
    // ******************************************************************************
    void Renderer2D::StartBatch()
    {
        s_Data->QuadIndexCount = 0;
        s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
        
        s_Data->TextureSlotIndex = 1;
    }

    // ******************************************************************************
    // Flush the batch
    // ******************************************************************************
    void Renderer2D::Flush()
    {
        // Nothing to draw
        if (s_Data->QuadIndexCount == 0)
            return;
        
        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data->QuadVertexBufferPtr - (uint8_t*)s_Data->QuadVertexBufferBase);
        s_Data->QuadVertexBuffer->SetData(s_Data->QuadVertexBufferBase, dataSize);
        
        // Bind textures
        for (uint32_t i = 0; i < s_Data->TextureSlotIndex; i++)
        {
            if (i > 0)
                RendererStatistics::TextureCount++;
            s_Data->TextureSlots[i]->Bind(i);
        }
        
        // Render the Scene
        Renderer::DrawIndexed(s_Data->QuadVertexArray, s_Data->QuadIndexCount);
    }

    // ******************************************************************************
    // New batch
    // ******************************************************************************
    void Renderer2D::NextBatch()
    {
        IK_CORE_WARN("Starts the new batch");

        // if num Quad per Batch exceeds then Render the Scene and reset all parameters
        EndScene();
        
        s_Data->QuadIndexCount = 0;
        s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
        
        s_Data->TextureSlotIndex = 1;
    }
    
    /* -------------------------- Premitives -------------------------------------------------*/
    
    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int32_t entID)
    {
        constexpr size_t    quadVertexCount = 4;
        const float         textureIndex    = 0.0f; // White Texture
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        const float         tilingFactor    = 1.0f;
        
        if (s_Data->QuadIndexCount >= Renderer2DData::MaxIndices)
            NextBatch();

        for (size_t i = 0; i < quadVertexCount; i++)
        {
            s_Data->QuadVertexBufferPtr->Position        = transform * s_Data->QuadVertexPositions[i];
            s_Data->QuadVertexBufferPtr->Color           = color;
            s_Data->QuadVertexBufferPtr->TexCoord        = textureCoords[i];
            s_Data->QuadVertexBufferPtr->TexIndex        = textureIndex;
            s_Data->QuadVertexBufferPtr->TilingFactor    = tilingFactor;
            s_Data->QuadVertexBufferPtr->ObjectID        = (int32_t)entID;
            s_Data->QuadVertexBufferPtr++;
        }
        
        s_Data->QuadIndexCount += 6;
        
        RendererStatistics::VertexCount += 4;
        RendererStatistics::IndexCount += 6;
    }
    
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, int32_t entID)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, color, entID);
    }
    
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int32_t entID)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        
        DrawQuad(transform, color, entID);
    }
    
    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color, int32_t entID)
    {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color, entID);
    }
    
    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color, int32_t entID)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        
        DrawQuad(transform, color, entID);
    }
    
    /* ---------------------------------------- Rendering Texture ---------------------------------------------------------------------*/
    
    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        constexpr size_t    quadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        
        if (s_Data->QuadIndexCount >= Renderer2DData::MaxIndices)
            NextBatch();

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data->TextureSlotIndex; i++)
        {
            if (*s_Data->TextureSlots[i] == *texture)
            {
                textureIndex = (float)i;
                break;
            }
        }
        
        if (textureIndex == 0.0f)
        {
            if (s_Data->TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
                NextBatch();

            textureIndex = (float)s_Data->TextureSlotIndex;
            s_Data->TextureSlots[s_Data->TextureSlotIndex] = texture;
            s_Data->TextureSlotIndex++;
        }
        
        for (size_t i = 0; i < quadVertexCount; i++)
        {
            s_Data->QuadVertexBufferPtr->Position        = transform * s_Data->QuadVertexPositions[i];
            s_Data->QuadVertexBufferPtr->Color           = tintColor;
            s_Data->QuadVertexBufferPtr->TexCoord        = textureCoords[i];
            s_Data->QuadVertexBufferPtr->TexIndex        = textureIndex;
            s_Data->QuadVertexBufferPtr->TilingFactor    = tilingFactor;
            s_Data->QuadVertexBufferPtr++;
        }
        
        s_Data->QuadIndexCount += 6;
        
        RendererStatistics::VertexCount += 4;
        RendererStatistics::IndexCount += 6;
    }
    
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
    }
    
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        
        DrawQuad(transform, texture, tilingFactor, tintColor);
    }
    
    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
    }
    
    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        
        DrawQuad(transform, texture, tilingFactor, tintColor);
    }
    
    /* ---------------------------------------- Rendering SubTexture from Sprite ---------------------------------------------------------------------*/
    
    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture>& subTexture, float tilingFactor, const glm::vec4& tintColor)
    {
        if (s_Data->QuadIndexCount >= Renderer2DData::MaxIndices)
            NextBatch();

        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
        
        const glm::vec2* textureCoords = subTexture->GetTexCoord();
        
        float textureIndex = 0.0f;
        
        for (int i = 1; i < s_Data->TextureSlotIndex; i++)
        {
            if (*s_Data->TextureSlots[i] == *(subTexture->GetTeture())) // if (s_Data->TextureSlots[i] == texture) will compare the shared pointer only
            {
                textureIndex = (float)i;
                break;
            }
        }
        
        if (0.0f == textureIndex)
        {
            textureIndex = (float)s_Data->TextureSlotIndex;
            s_Data->TextureSlots[s_Data->TextureSlotIndex] = subTexture->GetTeture();
            s_Data->TextureSlotIndex++;
        }
        
        for (size_t i = 0; i < quadVertexCount; i++)
        {
            s_Data->QuadVertexBufferPtr->Position     = transform * s_Data->QuadVertexPositions[i];
            s_Data->QuadVertexBufferPtr->Color        = color;
            s_Data->QuadVertexBufferPtr->TexCoord     = textureCoords[i];
            s_Data->QuadVertexBufferPtr->TexIndex     = textureIndex;
            s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
            s_Data->QuadVertexBufferPtr++;
        }
        s_Data->QuadIndexCount += 6;
        
        RendererStatistics::VertexCount += 4;
        RendererStatistics::IndexCount += 6;
    }
    
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture>& subTexture, float tilingFactor, const glm::vec4& tintColor)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, subTexture, tilingFactor, tintColor);
    }
    
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture>& subTexture, float tilingFactor, const glm::vec4& tintColor)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        
        DrawQuad(transform, subTexture, tilingFactor, tintColor);
    }
    
    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture>& subTexture, float tilingFactor, const glm::vec4& tintColor)
    {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subTexture, tilingFactor, tintColor);
    }
    
    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture>& subTexture, float tilingFactor, const glm::vec4& tintColor)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
        * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        
        DrawQuad(transform, subTexture, tilingFactor, tintColor);
    }
    
}
