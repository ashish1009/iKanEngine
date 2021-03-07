// ******************************************************************************
//   File    : OpenGlFrameBuffer.cpp
//   Project : i-Kan : Platform
//
//   Created by Ashish
// ******************************************************************************

#include <iKan/Platform/OpenGL/OpenGLFrameBuffer.h>
#include <glad/glad.h>

namespace iKan {

    namespace Utils {

        // ******************************************************************************
        // Get the Open GL Texture target with number of samples
        // ******************************************************************************
        static GLenum TextureTarget(bool multisampled)
        {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        // ******************************************************************************
        // Create the Open GL Frame buffer Texture
        // ******************************************************************************
        static void CreateTextures(uint32_t* outID, uint32_t count)
        {
            IK_CORE_INFO("OpenGL Frame buffer texture created with count {0}", count);

            glGenTextures(count, outID);
        }

        // ******************************************************************************
        // Bind the Open GL Frame buffer Texture
        // ******************************************************************************
        static void BindTexture(bool multisampled, uint32_t id)
        {
            glBindTexture(TextureTarget(multisampled), id);
        }

        // ******************************************************************************
        // Attach the color texture to the Open GL Framebuffer
        // ******************************************************************************
        static void AttachColorTexture(uint32_t id, int32_t samples, GLenum format, uint32_t width, uint32_t height, int32_t index)
        {
            IK_CORE_INFO("Attaching Color texture to the framebuffer. Samples: {0}, format: {1}, width: {2}, height: {3}, index{4} ", samples, (int32_t)format, width, height, index);

            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
        }

        // ******************************************************************************
        // Attach the Depth texture to the Open GL Framebuffer
        // ******************************************************************************
        static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
        {
            IK_CORE_INFO("Attaching Depth texture to the framebuffer. Samples: {0}, format: {1}, attachment type: {2}, width: {3}, height: {4}", samples, (int32_t)format, (int32_t)attachmentType, width, height);

            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
        }

        // ******************************************************************************
        // Attach the ID texture to the Open GL Framebuffer
        // ******************************************************************************
        static void AttachIDTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
        {
            IK_CORE_INFO("Attaching ID texture to the framebuffer. Samples: {0}, format: {1}, attachment type: {2}, width: {3}, height: {4}", samples, (int32_t)format, (int32_t)attachmentType, width, height);

            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, id, 0);

            GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
            glDrawBuffers(2, drawBuffers);
        }

        // ******************************************************************************
        // Check the rpecence of Depth Texture
        // ******************************************************************************
        static bool IsDepthFormat(FbTextureFormat format)
        {
            switch (format)
            {
                case FbTextureFormat::None:   return false;
                case FbTextureFormat::RGBA8:  return false;
                case FbTextureFormat::R32I:   return false;
                case FbTextureFormat::DEPTH24STENCIL8:  return true;
            }

            return false;
        }

    }
    
    // ******************************************************************************
    // Constructor Open GL Framebuffer
    // ******************************************************************************
    OpenGLFrameBuffer::OpenGLFrameBuffer(const FramebufferSpecification& spec)
    : m_Specifications(spec)
    {
        IK_CORE_INFO("Open GL Framebuffer constructed ");

        for (auto spec : m_Specifications.Attachments.Attachments)
        {
            if (!Utils::IsDepthFormat(spec.TextureFormat))
            {
                m_ColorAttachmentSpecifications.emplace_back(spec);
            }
            else
            {
                m_DepthAttachmentSpecification = spec;
            }
        }

        Invalidate();
    }
    
    // ******************************************************************************
    // Destructor Open GL Framebuffer
    // ******************************************************************************
    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        IK_CORE_WARN("Open GL Framebuffer Destroyed ");

        glDeleteFramebuffers(1, &m_RendererId);
        glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);
    }
    
    // ******************************************************************************
    // Invalidate Open GL Framebuffer
    // ******************************************************************************
    void OpenGLFrameBuffer::Invalidate()
    {
        IK_CORE_INFO("Invalidate Open GL Framebuffer");

        if (m_RendererId)
        {
            glDeleteFramebuffers(1, &m_RendererId);
            glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
            glDeleteTextures(1, &m_DepthAttachment);

            m_ColorAttachments.clear();
            m_DepthAttachment = 0;
        }
        
        glGenFramebuffers(1, &m_RendererId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

        bool multisample = m_Specifications.Samples > 1;

        // Attachments
        if (m_ColorAttachmentSpecifications.size())
        {
            m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
            Utils::CreateTextures(m_ColorAttachments.data(), (uint32_t)m_ColorAttachments.size());

            for (size_t i = 0; i < m_ColorAttachments.size(); i++)
            {
                Utils::BindTexture(multisample, m_ColorAttachments[i]);
                switch (m_ColorAttachmentSpecifications[i].TextureFormat)
                {
                    case FbTextureFormat::DEPTH24STENCIL8:
                    case FbTextureFormat::None: break;

                    case FbTextureFormat::RGBA8:
                        Utils::AttachColorTexture(m_ColorAttachments[i], m_Specifications.Samples, GL_RGBA8, m_Specifications.Width, m_Specifications.Height, (uint32_t)i);
                        break;

                    case FbTextureFormat::R32I:
                        Utils::AttachIDTexture(m_ColorAttachments[i], m_Specifications.Samples, GL_R32I, GL_COLOR_ATTACHMENT1, m_Specifications.Width, m_Specifications.Height);
                        break;
                }
            }
        }

        if (m_DepthAttachmentSpecification.TextureFormat != FbTextureFormat::None)
        {
            Utils::CreateTextures(&m_DepthAttachment, 1);
            Utils::BindTexture(multisample, m_DepthAttachment);
            switch (m_DepthAttachmentSpecification.TextureFormat)
            {
                case FbTextureFormat::RGBA8:
                case FbTextureFormat::None:
                case FbTextureFormat::R32I:
                    break;

                case FbTextureFormat::DEPTH24STENCIL8:
                    // TODO: IT WAS "GL_DEPTH_STENCIL_ATTACHMENT" but was not working
                    Utils::AttachDepthTexture(m_DepthAttachment, m_Specifications.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_ATTACHMENT, m_Specifications.Width, m_Specifications.Height);
                    break;
            }
        }

        if (m_ColorAttachments.size() > 1)
        {
            IK_CORE_ASSERT((m_ColorAttachments.size() <= 4), "Inalid Attachment");
            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            glDrawBuffers((GLsizei)m_ColorAttachments.size(), buffers);
        }
        else if (m_ColorAttachments.empty())
        {
            // Only depth-pass
            glDrawBuffer(GL_NONE);
        }

        IK_CORE_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "FrameBuffer is Incomplete ");
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    // ******************************************************************************
    // Bind Open GL Framebuffer
    // ******************************************************************************
    void OpenGLFrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
        glViewport(0, 0, m_Specifications.Width, m_Specifications.Height);
    }
    
    // ******************************************************************************
    // Unbind Open GL Framebuffer
    // ******************************************************************************
    void OpenGLFrameBuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    // ******************************************************************************
    // Resize Open GL Framebuffer
    // ******************************************************************************
    void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
    {
        IK_CORE_INFO("Resozing the Framebuffer");

        m_Specifications.Width = width;
        m_Specifications.Height = height;
        
        Invalidate();
    }
        
}
