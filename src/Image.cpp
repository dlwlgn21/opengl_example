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
Image::~Image()
{
    if (mpData != nullptr)
    {
        stbi_image_free(mpData);
    }
}

bool Image::TryLoadWithStb(const string& filePath)
{
    mpData = stbi_load(filePath.c_str(), &mWidth, &mHeight, &mChannelCount, 0);
    if (mpData == nullptr)
    {
        SPDLOG_ERROR("Failed To Load Image: {}", filePath);
        return false;
    }
    return true;
}