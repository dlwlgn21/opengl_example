#include "Texture.h"

using namespace std;

unique_ptr<Texture> Texture::CreateEmptyTexture(int width, int height, uint32_t format)
{
    unique_ptr<Texture> tex = unique_ptr<Texture>(new Texture());
    tex->CreateTexture();
    tex->SetTextureFormat(width, height, format);
    tex->SetFilter(GL_LINEAR, GL_LINEAR);
    return std::move(tex);
}

unique_ptr<Texture> Texture::Create(int width, int height, uint32_t format, uint32_t type)
{
    unique_ptr<Texture> texture = unique_ptr<Texture>(new Texture());
    texture->CreateTexture();
    texture->SetTextureFormat(width, height, format, type);
    texture->SetFilter(GL_LINEAR, GL_LINEAR);
    return std::move(texture);
}

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
    SetFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}
void Texture::SetTextureFormat(int width, int height, uint32_t format)
{
    mWidth = width;
    mHeight = height;
    mFormat = format;
    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        mFormat,
        mWidth, 
        mHeight, 
        0,
        mFormat, 
        GL_UNSIGNED_BYTE,
        nullptr
    );
}
void Texture::SetTextureFormat(int width, int height, uint32_t format, uint32_t type)
{
    mWidth = width;
    mHeight = height;
    mFormat = format;
    mType = type;
    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        mFormat,
        mWidth, 
        mHeight, 
        0,
        mFormat, 
        mType,
        nullptr
    );
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
	mWidth = pImg->GetWidth();
    mHeight = pImg->GetHeight();
    mFormat = format;
    mType = GL_UNSIGNED_BYTE;
    glTexImage2D(
        GL_TEXTURE_2D,      // Target
        0,                  // GPU 쪽의 텍스쳐 데이터 기술 - Mip-Map 레벨값
        mFormat,            // GPU 쪽의 텍스쳐 데이터 기술 - GPU 쪽에서 유지 하려고 하는 채널의 타입
        mWidth,             // GPU 쪽의 텍스쳐 데이터 기술
        mHeight,            // GPU 쪽의 텍스쳐 데이터 기술
        0,                  // GPU 쪽의 텍스쳐 데이터 기술 - Border size
        mFormat,            // CPU 할당된 이미지 데이터 기술 - image의 픽셀 타입. 채널
        mType,              // CPU 할당된 이미지 데이터 기술 - image가 하나의 채널을 표현하는데 쓰는 데이터 타입
        pImg->GetData()     // CPU 할당된 이미지 데이터 기술 - 실제 데이터가 들어가 있는 포인터 넘겨줌
    );
    glGenerateMipmap(GL_TEXTURE_2D);
}


