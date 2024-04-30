#include <VulkanRHI/public/VulkanImage.h>
#include <VulkanRHI/public/VulkanDevice.h>
//Vulkan Image

uint32_t VulkanImage::GetImageLayerCount(VkImageViewType inType, uint32_t inNumArraySlices)
{
	switch (inType)
	{
	case VK_IMAGE_VIEW_TYPE_3D:
		return 1;
		break;
	case VK_IMAGE_VIEW_TYPE_CUBE:
		if (inNumArraySlices != 1)
		{
			std::runtime_error("Error! Cube can only has one slice!");
		}
		return 6;
		break;
	case VK_IMAGE_VIEW_TYPE_CUBE_ARRAY:
		return 6 * inNumArraySlices;
		break;
	case VK_IMAGE_VIEW_TYPE_1D_ARRAY:
	case VK_IMAGE_VIEW_TYPE_2D_ARRAY:
		return inNumArraySlices;
		break;
	default:
		return 1;
		break;
	}
}


//Vulkan Image View
shared_ptr<VulkanImageView> VulkanImageView::Create(shared_ptr<VulkanDevice> inDevice,
	VkImage inImg, VkImageViewType inViewType,
	VkImageAspectFlags inAspectFlag, VkFormat inFormat,
	uint32_t inFirstMip, uint32_t inNumMips,
	uint32_t inArraySliceIndex, uint32_t inNumArraySlices,
	bool bUseIdentitySwizzle, VkImageUsageFlags inImageUsageFlags)
{
	shared_ptr<VulkanImageView> newImageView(new VulkanImageView(inDevice, inImg, inViewType, inAspectFlag, inFormat, inFirstMip, inNumMips, inArraySliceIndex, inNumArraySlices, bUseIdentitySwizzle, inImageUsageFlags));
}

VulkanImageView::VulkanImageView(shared_ptr<VulkanDevice> inDevice,
	VkImage inImg, VkImageViewType inViewType,
	VkImageAspectFlags inAspectFlag, VkFormat inFormat,
	uint32_t inFirstMip, uint32_t inNumMips,
	uint32_t inArraySliceIndex, uint32_t inNumArraySlices,
	bool bUseIdentitySwizzle, VkImageUsageFlags inImageUsageFlags)
{
	VkImageViewCreateInfo imageViewCreateInfo;
	InitializeVkStructture(imageViewCreateInfo, VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO);
	imageViewCreateInfo.image = inImg;
	imageViewCreateInfo.subresourceRange.baseMipLevel = inFirstMip;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = inArraySliceIndex;
	imageViewCreateInfo.subresourceRange.layerCount = VulkanImage::GetImageLayerCount(inViewType, inNumArraySlices);
	imageViewCreateInfo.subresourceRange.levelCount = inNumMips;
	imageViewCreateInfo.subresourceRange.aspectMask = inAspectFlag;
	imageViewCreateInfo.viewType = inViewType;
	imageViewCreateInfo.format = inFormat;
	if (bUseIdentitySwizzle)
	{
		imageViewCreateInfo.components.r = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.g = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.b = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.a = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY;
	}
	else
	{

	}
	//imageViewCreateInfo.flags = 
	vkCreateImageView(inDevice->GetDevice(), &imageViewCreateInfo, nullptr, &mImgView);

}
