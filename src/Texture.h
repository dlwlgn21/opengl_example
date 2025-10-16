#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Image.h"

CLASS_PTR(Texture)
class Texture
{
public:
    static std::unique_ptr<Texture> CreateFromImg(const Image* pImg);
    ~Texture();

    const uint32_t GetId() const { return mTextureId; }
    void Bind() const;
    void SetFilter(uint32_t minFilter, uint32_t magFilter) const;
    void SetWrapMode(uint32_t sWrap, uint32_t tWrap) const;
private:
    Texture() = default;
    void CreateTexture();
    void SetTextureFromImg(const Image* pImg);
    uint32_t mTextureId{};
};


#endif //__TEXTURE_H__