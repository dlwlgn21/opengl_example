#pragma once
#include "Common.h"
#include "Image.h"

CLASS_PTR(CubeTexture)
class CubeTexture
{
public:
    static std::unique_ptr<CubeTexture> CreateFromImages(const std::vector<Image*>& images);
    ~CubeTexture();

    const uint32_t GetId() const { return mTexId; }
    void Bind() const;
private:
    CubeTexture() {}
    bool TryInitFromImages(const std::vector<Image*>& images);
    uint32_t mTexId { 0 };
};