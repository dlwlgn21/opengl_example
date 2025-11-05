#include "Framebuffer.h"

using namespace std;
unique_ptr<Framebuffer> Framebuffer::CreateOrNull(vector<unique_ptr<Texture>>& colorAttachmentTextures)
{
    unique_ptr<Framebuffer> frameBuffer = unique_ptr<Framebuffer>(new Framebuffer());
    if (!frameBuffer->TryInitWithColorAttachmentTexture(colorAttachmentTextures))
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

bool Framebuffer::TryInitWithColorAttachmentTexture(vector<unique_ptr<Texture>>& colorAttachmentTextures)
{
    mColorAttachmentTextures.reserve(colorAttachmentTextures.size());
    for (size_t i = 0; i < colorAttachmentTextures.size(); i++)
    {
        mColorAttachmentTextures.push_back(std::move(colorAttachmentTextures[i]));
    }
    glGenFramebuffers(1, &mFramebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferId);

    for (size_t i = 0; i < mColorAttachmentTextures.size(); i++) 
    {
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(i), 
            GL_TEXTURE_2D,
            mColorAttachmentTextures[i]->GetId(), 
            0
        );
    }

    if (mColorAttachmentTextures.size() > 0)
    {
        std::vector<GLenum> attachments;
        attachments.resize(mColorAttachmentTextures.size());
        for (size_t i = 0; i < mColorAttachmentTextures.size(); i++)
        {
            attachments[i] = GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(i);
        }
        glDrawBuffers(static_cast<GLsizei>(mColorAttachmentTextures.size()), attachments.data());
    }

    int width = mColorAttachmentTextures[0]->GetWidth();
    int height = mColorAttachmentTextures[0]->GetHeight();
 
    glGenRenderbuffers(1, &mDepthStencilBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilBufferId);
    glRenderbufferStorage(
        GL_RENDERBUFFER, 
        GL_DEPTH24_STENCIL8,
        width,
        height
    );
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