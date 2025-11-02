#include "ShadowMap.h"

using namespace std;

unique_ptr<ShadowMap> ShadowMap::Create(int width, int height)
{
    unique_ptr<ShadowMap> shadowMap = unique_ptr<ShadowMap>(new ShadowMap());
    if (!shadowMap->TryInit(width, height))
    {
        return nullptr;
    }
    return std::move(shadowMap);
}

ShadowMap::~ShadowMap()
{
  if (mFrameBuffuerId)
  {
      glDeleteFramebuffers(1, &mFrameBuffuerId);
  }
}

void ShadowMap::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffuerId);
}

bool ShadowMap::TryInit(int width, int height)
{
    glGenFramebuffers(1, &mFrameBuffuerId);
    Bind();

    mShadowMap = Texture::Create(width, height, GL_DEPTH_COMPONENT, GL_FLOAT);
    mShadowMap->SetFilter(GL_NEAREST, GL_NEAREST);
    mShadowMap->SetWrapMode(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        mShadowMap->GetId(), 
        0
    );
    glDrawBuffer(GL_NONE); // color attachment가 없음을 OpenGL에게 알려줌
    glReadBuffer(GL_NONE); // color attachment가 없음을 OpenGL에게 알려줌
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        SPDLOG_ERROR("failed to complete shadow map framebuffer: {:x}", status);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}