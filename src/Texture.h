#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Image.h"

CLASS_PTR(Texture)
class Texture
{
public:
    static std::unique_ptr<Texture> CreateEmptyTexture(int width, int height, uint32_t format);
    static std::unique_ptr<Texture> CreateFromImg(const Image* pImg);
    ~Texture();

    const uint32_t GetId() const { return mTextureId; }
    void Bind() const;
    void SetFilter(uint32_t minFilter, uint32_t magFilter) const;
    void SetWrapMode(uint32_t sWrap, uint32_t tWrap) const;

    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }
    uint32_t GetFormat() const { return mFormat; }
private:
    Texture() = default;
    void CreateTexture();
    void SetTextureFromImg(const Image* pImg);
    void SetTextureFormat(int width, int height, uint32_t format);

    uint32_t mTextureId{};

    int mWidth{};
    int mHeight{};
    uint32_t mFormat{ GL_RGBA };
};


#endif //__TEXTURE_H__