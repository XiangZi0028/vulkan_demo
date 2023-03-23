#pragma once
#include "vulkan/vulkan.hpp"

class GVKTexture
{
public:
    GVKTexture();
    ~GVKTexture();
private:
    int ImgWidth = 0;
    int ImgHeight = 0;
    VkImage mImage;
    VkImageView mImageView;
};