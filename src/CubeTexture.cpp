#include "CubeTexture.h"
using namespace std;
unique_ptr<CubeTexture> CubeTexture::CreateFromImages(const std::vector<Image*>& images)
{
    unique_ptr<CubeTexture> texture = unique_ptr<CubeTexture>(new CubeTexture());
    if (!texture->TryInitFromImages(images))
    {
        return nullptr;
    }
    return std::move(texture);
}

CubeTexture::~CubeTexture()
{
    if (mTexId)
    {
        glDeleteTextures(1, &mTexId);
    }
}

void CubeTexture::Bind() const
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexId);    
}

bool CubeTexture::TryInitFromImages(const std::vector<Image*>& images)
{
    glGenTextures(1, &mTexId);
    Bind();

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (uint32_t i = 0; i < (uint32_t)images.size(); i++)
    {
        Image* pImage = images[i];
        GLenum format = GL_RGBA;
        switch (pImage->GetChannelCount())
        {
            default: break;
            case 1: format = GL_RED; break;
            case 2: format = GL_RG; break;
            case 3: format = GL_RGB; break;
        }

        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
            0, 
            GL_RGB,
            pImage->GetWidth(), 
            pImage->GetHeight(), 0,
            format, GL_UNSIGNED_BYTE,
            pImage->GetData()
        );
    }
    return true;
}