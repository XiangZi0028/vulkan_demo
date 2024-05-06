#include <VulkanRHI/public/VulkanImage.h>
#include <VulkanRHI/public/VulkanDevice.h>
#include <vulkan/vulkan.h>
//Vulkan Image
VulkanImage::~VulkanImage()
{
	if (mImgView)
	{
		mImgView.reset();
	}
	if (mImg != VK_NULL_HANDLE)
	{
		vkDestroyImage(mDevice->GetDevice(), mImg, nullptr);
	}
	if (mDeviceMemroy != VK_NULL_HANDLE)
	{
		vkFreeMemory(mDevice->GetDevice(), mDeviceMemroy, nullptr);
	}
};
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
void VulkanImage::InitImage(uint32_t inWidth, uint32_t inHeight, VkFormat inFormat)
{
	mImageSize.width = inWidth;
	mImageSize.height = inHeight;
	mFormat = inFormat;
};
shared_ptr<VulkanImage> VulkanImage::CreateTexture2D(shared_ptr<VulkanDevice> inDevice, uint32_t inWidth, uint32_t inHeight, VkImageUsageFlags inUsage, VkFormat inFormat, uint32_t inMipLevels, VkImageAspectFlags inImageAspect,  VkSampleCountFlagBits inSampleCount)
{
	//Create Image
	VkImageCreateInfo imgCreateInfo;
	ZeroVulkanStruct(imgCreateInfo, VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO);
	imgCreateInfo.imageType = VkImageType::VK_IMAGE_TYPE_2D;
	imgCreateInfo.extent.width = inWidth;
	imgCreateInfo.extent.height = inHeight;
	imgCreateInfo.extent.depth = 1;
	imgCreateInfo.format = inFormat;
	imgCreateInfo.arrayLayers = 1;
	imgCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imgCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imgCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imgCreateInfo.samples = inSampleCount;
	imgCreateInfo.mipLevels = inMipLevels;
	imgCreateInfo.usage = inUsage;

	VkImage newImage = VK_NULL_HANDLE;
	vkCreateImage(inDevice->GetDevice(), &imgCreateInfo, nullptr, &newImage);
	//Allocated Memory And Bind Image And Memory 
	VkMemoryRequirements imgMemoryRequirement;
	vkGetImageMemoryRequirements(inDevice->GetDevice(), newImage, &imgMemoryRequirement);
	VkMemoryAllocateInfo memAllocInfo;
	ZeroVulkanStruct(memAllocInfo, VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO);
	memAllocInfo.allocationSize = imgMemoryRequirement.size;
	VkPhysicalDeviceMemoryProperties gpuMemoryProperties;
	// The returned structure contains memoryTypes and memoryHeaps members
	vkGetPhysicalDeviceMemoryProperties(inDevice->GetGpu(), &gpuMemoryProperties);
	// Find available memory type Index
	int memoryTypeIndex = -1;
	for (uint32_t Index = 0; Index < gpuMemoryProperties.memoryTypeCount; Index++) {
		if ((imgMemoryRequirement.memoryTypeBits & (1 << Index)) && (gpuMemoryProperties.memoryTypes[Index].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
			memoryTypeIndex = Index;
			break;
		}
	}
	if (memoryTypeIndex == -1)
	{
		throw std::runtime_error("failed to find suitable memory type!");
	}
	VkDeviceMemory newMemory;
	memAllocInfo.memoryTypeIndex = memoryTypeIndex;
	vkAllocateMemory(inDevice->GetDevice(), &memAllocInfo, nullptr, &newMemory);
	vkBindImageMemory(inDevice->GetDevice(), newImage, newMemory, 0);
	shared_ptr<VulkanImageView> newImageView = VulkanImageView::Create(inDevice, newImage, VK_IMAGE_VIEW_TYPE_2D, inImageAspect, inFormat, 0,  inMipLevels, 0, 1 );
	shared_ptr<VulkanImage> newVulkanImage(new VulkanImage(inDevice, newImageView, newImage, newMemory));
	newVulkanImage->InitImage(inWidth, inHeight, inFormat);
	return  newVulkanImage;
}


shared_ptr<VulkanImage> VulkanImage::CreateAttachment(shared_ptr<VulkanDevice> inDevice, uint32_t inWidth, uint32_t inHeight,
	VkFormat inFormat,
	VkImageUsageFlags inUsage,
	VkImageAspectFlags inImageAspect)
{
	shared_ptr<VulkanImage> newAttachment = CreateTexture2D(inDevice, inWidth, inHeight, inUsage, inFormat, 1, inImageAspect);
	return newAttachment;
}


//Vulkan Image View

VulkanImageView::~VulkanImageView()
{
	if (mImgView != VK_NULL_HANDLE)
	{
		vkDestroyImageView(mDevice->GetDevice(), mImgView, nullptr);
	}
};
shared_ptr<VulkanImageView> VulkanImageView::Create(shared_ptr<VulkanDevice> inDevice,
	VkImage inImg, VkImageViewType inViewType,
	VkImageAspectFlags inAspectFlag, VkFormat inFormat,
	uint32_t inFirstMip, uint32_t inNumMips,
	uint32_t inArraySliceIndex, uint32_t inNumArraySlices,
	bool bUseIdentitySwizzle, VkImageUsageFlags inImageUsageFlags)
{
	shared_ptr<VulkanImageView> newImageView(new VulkanImageView(inDevice, inImg, inViewType, inAspectFlag, inFormat, inFirstMip, inNumMips, inArraySliceIndex, inNumArraySlices, bUseIdentitySwizzle, inImageUsageFlags));
	return newImageView;
}

VulkanImageView::VulkanImageView(shared_ptr<VulkanDevice> inDevice,
	VkImage inImg, VkImageViewType inViewType,
	VkImageAspectFlags inAspectFlag, VkFormat inFormat,
	uint32_t inFirstMip, uint32_t inNumMips,
	uint32_t inArraySliceIndex, uint32_t inNumArraySlices,
	bool bUseIdentitySwizzle, VkImageUsageFlags inImageUsageFlags)
	:mDevice(inDevice)
{
	VkImageViewCreateInfo imageViewCreateInfo;
	ZeroVulkanStruct(imageViewCreateInfo, VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO);
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
