#ifndef VLUKANDEMO_GVKTEXTURE_H
#define VLUKANDEMO_GVKGVKSWAPCHAIN_H

#include "vulkan/vulkan.hpp"

class GVKTexture
{
public:
    GVKTexture(int ImgWidth, int ImgHeight);
    ~GVKTexture();
    VkImageView GetImageView()const  {return mImageView;};
    void SetVKImage(VkImage InImage){ mImage = InImage; };
    void SetVKImageView(VkImageView ImageView){mImageView = ImageView;};
    void SetVKImageAndView(VkImage InImage, VkImageView InImageView){mImage = InImage; mImageView = InImageView; };
private:
    int mImgWidth = 0;
    int mImgHeight = 0;
    VkImage mImage;
    VkImageView mImageView;
};

#endif //VLUKANDEMO_GVKGVKSWAPCHAIN_H