#pragma once
#include <vulkan/vulkan.h>
#include <CommonMicro.h>
#include <VulkanCommonDefine.h>

class VulkanDevice;
class VulkanImageView;
class VulkanCommandBuffer;
class VulkanImage : public enable_shared_from_this<VulkanImage>
{
public:
	~VulkanImage();
	static shared_ptr<VulkanImage> CreateAttachment(shared_ptr<VulkanDevice> inDevice, uint32_t inWidth, uint32_t inHeight,
		VkFormat inFormat,
		VkImageUsageFlags inUsage,
		VkImageAspectFlags inImageAspect);

	static shared_ptr<VulkanImage> CreateSwapChainAttachment(shared_ptr<VulkanDevice> inDevice, uint32_t inWidth, uint32_t inHeight,
		VkFormat inFormat,
		VkImageUsageFlags inUsage,
		VkImageAspectFlags inImageAspect);

	static shared_ptr<VulkanImage> CreateTexture2D(shared_ptr<VulkanDevice> inDevice, uint32_t inWidth, uint32_t inHeight, 
		VkImageUsageFlags inUsage,
		VkFormat inFormat = VK_FORMAT_R8G8B8A8_SRGB,
		uint32_t MipLevels = 1,
		VkImageAspectFlags inImageAspect = VK_IMAGE_ASPECT_COLOR_BIT,
		VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT);

	static uint32_t GetImageLayerCount(VkImageViewType inType, uint32_t inNumArraySlices);

private:
	
	VulkanImage(shared_ptr<VulkanDevice> inDevice, shared_ptr<VulkanImageView> inImageView, VkImage inImage, VkDeviceMemory inVkMemory)
		: mDevice(inDevice)
		, mImgView(inImageView)
		, mImg(inImage)
		, mDeviceMemroy(inVkMemory)
	{

	};
	void InitImage(uint32_t inWidth, uint32_t inHeight, VkFormat inFormat);
	DefineMemberWithGetter(VkExtent2D, ImageSize);
	DefineMemberWithGetter(VkImage, Img);
	DefineMemberWithGetter(VkDeviceMemory, DeviceMemroy);
	DefineMemberWithGetter(shared_ptr<VulkanImageView>, ImgView);
	DefineMemberWithGetter(shared_ptr<VulkanDevice>, Device);
	DefineMemberWithGetter(VkFormat, Format);

};


class VulkanImageView : public enable_shared_from_this<VulkanImageView>
{
public:
	~VulkanImageView();

	static shared_ptr<VulkanImageView> Create(shared_ptr<VulkanDevice> inDevice,
		VkImage inImg,
		VkImageViewType ViewType,
		VkImageAspectFlags inAspectFlag,
		VkFormat inFormat,
		uint32_t inFirstMip,
		uint32_t inNumMips,
		uint32_t inArraySliceIndex,
		uint32_t inNumArraySlices,
		bool bUseIdentitySwizzle = true,
		VkImageUsageFlags inImageUsageFlags = 0);
private:
	
	VulkanImageView(shared_ptr<VulkanDevice> inDevice,
		VkImage inImg, VkImageViewType inViewType,
		VkImageAspectFlags inAspectFlag, VkFormat inFormat,
		uint32_t inFirstMip, uint32_t inNumMips,
		uint32_t inArraySliceIndex, uint32_t inNumArraySlices,
		bool bUseIdentitySwizzle, VkImageUsageFlags inImageUsageFlags);


	DefineMemberWithGetter(VkImage, Img);
	DefineMemberWithGetter(VkImageView, ImgView);
	DefineMemberWithGetter(shared_ptr<VulkanDevice>, Device);
};
