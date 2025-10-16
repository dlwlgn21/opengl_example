#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "Image.h"

using namespace std;
unique_ptr<Image> Image::LoadOrNull(const string& filePath)
{
    unique_ptr<Image> image = unique_ptr<Image>(new Image());
    if (!image->TryLoadWithStb(filePath))
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
bool Image::TryLoadWithStb(const string& filePath)
{
    stbi_set_flip_vertically_on_load(true);
    mpData = stbi_load(filePath.c_str(), &mWidth, &mHeight, &mChannelCount, 0);
    if (mpData == nullptr)
    {
        SPDLOG_ERROR("Failed To Load Image: {}", filePath);
        return false;
    }
    return true;
}
