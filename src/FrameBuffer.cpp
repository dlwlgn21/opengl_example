#include "Framebuffer.h"

using namespace std;
unique_ptr<Framebuffer> Framebuffer::CreateOrNull(std::unique_ptr<Texture> pColorAttachmentTexture)
{
    unique_ptr<Framebuffer> frameBuffer = unique_ptr<Framebuffer>(new Framebuffer());
    if (!frameBuffer->TryInitWithColorAttachmentTexture(std::move(pColorAttachmentTexture)))
    {
        return nullptr;
    }
    return std::move(frameBuffer);
}

Framebuffer::~Framebuffer()
{
    if (mDepthStencilBufferId)
    {
        glDeleteRenderbuffers(1, &mDepthStencilBufferId);
    }
    if (mFramebufferId)
    {
        glDeleteFramebuffers(1, &mFramebufferId);
    }
}

void Framebuffer::BindToDefault()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferId);
}

bool Framebuffer::TryInitWithColorAttachmentTexture(std::unique_ptr<Texture> pColorAttachmentTexture)
{
    mpColorAttachmentTexture = std::move(pColorAttachmentTexture);
    glGenFramebuffers(1, &mFramebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferId);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        mpColorAttachmentTexture->GetId(),
        0
    );

    glGenRenderbuffers(1, &mDepthStencilBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilBufferId);
    glRenderbufferStorage(
        GL_RENDERBUFFER, 
        GL_DEPTH24_STENCIL8,
        mpColorAttachmentTexture->GetWidth(),
        mpColorAttachmentTexture->GetHeight());
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER,
        mDepthStencilBufferId
    );

    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (result != GL_FRAMEBUFFER_COMPLETE)
    {
        SPDLOG_ERROR("failed to create framebuffer: {}", result);
        return false;
    }

    BindToDefault();

    return true;
}