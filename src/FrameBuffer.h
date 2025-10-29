#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "Common.h"
#include "Texture.h"

CLASS_PTR(Framebuffer);
class Framebuffer 
{
public:
    static std::unique_ptr<Framebuffer> CreateOrNull(std::unique_ptr<Texture> pColorAttachmentTexture);
    static void BindToDefault();
    ~Framebuffer();

    const uint32_t GetId() const { return mFramebufferId; }
    void Bind() const;
    const Texture* GetColorAttachment() const { return mpColorAttachmentTexture.get(); }

private:
    Framebuffer() = default;
    bool TryInitWithColorAttachmentTexture(std::unique_ptr<Texture> pColorAttachmentTexture);

    uint32_t mFramebufferId { 0 };
    uint32_t mDepthStencilBufferId { 0 };
    std::unique_ptr<Texture> mpColorAttachmentTexture { nullptr };
};

#endif // __FRAMEBUFFER_H__