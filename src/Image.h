#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "Common.h"

CLASS_PTR(Image)
class Image
{
public:
    static std::unique_ptr<Image> LoadOrNull(const std::string& filePath);
    static std::unique_ptr<Image> CreateOrNull(int width, int height, int channelCount = 4);
    static std::unique_ptr<Image> CreateSingleColorImageOrNull(int width, int height, const glm::vec4& color);
    ~Image();

    const uint8_t* GetData() const { return mpData; }
    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }
    int GetChannelCount() const { return mChannelCount; }

    void SetCheckImg(int gridX, int gridY);
private:
    Image() = default;
    bool TryLoadWithStb(const std::string& filePath);
    bool TryAllocate(int width, int height, int channelCount);
    int mWidth{};
    int mHeight{};
    int mChannelCount{};
    uint8_t* mpData{};
};


#endif //__IMAGE_H__