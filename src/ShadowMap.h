#pragma once
#include "texture.h"

CLASS_PTR(ShadowMap);
class ShadowMap 
{
public:
    static std::unique_ptr<ShadowMap> Create(int width, int height);
    ~ShadowMap();
    const uint32_t GetId() const { return mFrameBuffuerId; }
    void Bind() const;
    const Texture* GetShadowMap() const { return mShadowMap.get(); }

private:
    ShadowMap() = default;
    bool TryInit(int width, int height);

    uint32_t mFrameBuffuerId { 0 };   // depth map에 렌더링을 하기 위한 프레임버퍼
    std::unique_ptr<Texture> mShadowMap { nullptr };  // depth map 저장을 위한 텍스처 멤버
};