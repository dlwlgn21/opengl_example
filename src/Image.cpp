#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "Image.h"

using namespace std;
unique_ptr<Image> Image::LoadOrNull(const string& filePath, bool isFlipY)
{
    unique_ptr<Image> image = unique_ptr<Image>(new Image());
    if (!image->TryLoadWithStb(filePath, isFlipY))
    {
        return nullptr;
    }
    return move(image);
}

unique_ptr<Image> Image::CreateOrNull(int width, int height, int channelCount)
{
    unique_ptr<Image> image = unique_ptr<Image>(new Image());
    if (!image->TryAllocate(width, height, channelCount))
    {
        return nullptr;
    }
    return move(image);
}
ImageUPtr Image::CreateSingleColorImageOrNull(int width, int height, const glm::vec4& color)
{
    glm::vec4 clamped = glm::clamp(color * 255.0f, 0.0f, 255.0f);
    uint8_t rgba[4] =
    {
        (uint8_t)clamped.r, 
        (uint8_t)clamped.g, 
        (uint8_t)clamped.b, 
        (uint8_t)clamped.a, 
    };
    unique_ptr<Image> image = CreateOrNull(width, height, 4);
    for (int i = 0; i < width * height; i++)
    {
        memcpy(image->mpData + 4 * i, rgba, 4);
    }
    return std::move(image);
}

void Image::SetCheckImg(int gridX, int gridY)
{
    for (int i = 0; i < mHeight; ++i)
    {
        for (int j = 0; j < mWidth; ++j)
        {
            int pos = (i * mWidth + j) * mChannelCount;
            bool bIsEven = ((j / gridX) + (i / gridY)) % 2 == 0;
            uint8_t val = bIsEven ? 255 : 0;
            for (int k = 0; k < mChannelCount; ++k)
            {
                mpData[pos + k] = val;
            }
            if (mChannelCount > 3)
            {
                mpData[3] = 255;
            }
        }
    }

}
Image::~Image()
{
    if (mpData != nullptr)
    {
        stbi_image_free(mpData);
    }
}
bool Image::TryAllocate(int width, int height, int channelCount)
{
    mWidth = width;
    mHeight = height;
    mChannelCount = channelCount;
    mpData = (uint8_t*)malloc(mWidth * mHeight * mChannelCount);
    return mpData != nullptr ? true : false; 
}
bool Image::TryLoadWithStb(const string& filePath, bool isFlipY)
{
    stbi_set_flip_vertically_on_load(isFlipY);
    mpData = stbi_load(filePath.c_str(), &mWidth, &mHeight, &mChannelCount, 0);
    if (mpData == nullptr)
    {
        SPDLOG_ERROR("Failed To Load Image: {}", filePath);
        return false;
    }
    return true;
}
