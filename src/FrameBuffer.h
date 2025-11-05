#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "Common.h"
#include "Texture.h"

CLASS_PTR(Framebuffer);
class Framebuffer 
{
public:
    static std::unique_ptr<Framebuffer> CreateOrNull(std::vector<std::unique_ptr<Texture>>& colorAttachmentTextures);
    static void BindToDefault();
    ~Framebuffer();

    const uint32_t GetId() const { return mFramebufferId; }
    void Bind() const;
    int GetColorAttachmentCount() const { return static_cast<int>(mColorAttachmentTextures.size()); }
    const Texture* GetColorAttachmentAt(int idx) const { return mColorAttachmentTextures[idx].get(); }

private:
    Framebuffer() = default;
    bool TryInitWithColorAttachmentTexture(std::vector<std::unique_ptr<Texture>>& colorAttachmentTextures);

    uint32_t mFramebufferId { 0 };
    uint32_t mDepthStencilBufferId { 0 };
    std::vector<std::unique_ptr<Texture>> mColorAttachmentTextures;
};

#endif // __FRAMEBUFFER_H__