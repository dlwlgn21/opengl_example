#include "Texture.h"

using namespace std;

unique_ptr<Texture> Texture::CreateFromImg(const Image* pImg)
{
    assert(pImg != nullptr);
    unique_ptr<Texture> tex = unique_ptr<Texture>(new Texture());
    tex->CreateTexture();
    tex->SetTextureFromImg(pImg);
    return move(tex);
}
Texture::~Texture()
{
    if (mTextureId)
    {
        glDeleteTextures(1, &mTextureId);
    }
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, mTextureId);
}

void Texture::SetFilter(uint32_t minFilter, uint32_t magFilter) const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}
void Texture::SetWrapMode(uint32_t sWrap, uint32_t tWrap) const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap); // x coord
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap); // y coord
}
void Texture::CreateTexture()
{
    glGenTextures(1, &mTextureId);
    Bind();
    SetFilter(GL_LINEAR, GL_LINEAR);
    SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}
void Texture::SetTextureFromImg(const Image* pImg)
{
    assert(pImg != nullptr);
    GLenum format = GL_RGBA;
    switch (pImg->GetChannelCount())
    {
        case 1: format = GL_RED; break;
        case 2: format = GL_RG;  break;
        case 3: format = GL_RGB; break;
        default: break;
    }

    glTexImage2D(
        GL_TEXTURE_2D,      // Target
        0,                  // GPU 쪽의 텍스쳐 데이터 기술 - Mip-Map 레벨값
        GL_RGBA,            // GPU 쪽의 텍스쳐 데이터 기술 - GPU 쪽에서 유지 하려고 하는 채널의 타입
        pImg->GetWidth(),   // GPU 쪽의 텍스쳐 데이터 기술
        pImg->GetHeight(),  // GPU 쪽의 텍스쳐 데이터 기술
        0,                  // GPU 쪽의 텍스쳐 데이터 기술 - Border size
        format,             // CPU 할당된 이미지 데이터 기술 - image의 픽셀 타입. 채널
        GL_UNSIGNED_BYTE,   // CPU 할당된 이미지 데이터 기술 - image가 하나의 채널을 표현하는데 쓰는 데이터 타입
        pImg->GetData()     // CPU 할당된 이미지 데이터 기술 - 실제 데이터가 들어가 있는 포인터 넘겨줌
    );
}


