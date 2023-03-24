#ifndef VLUKANDEMO_GVKTEXTURE_H
#define VLUKANDEMO_GVKGVKSWAPCHAIN_H

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

#endif //VLUKANDEMO_GVKGVKSWAPCHAIN_H