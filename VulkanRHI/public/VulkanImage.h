#pragma once
#include <vulkan/vulkan.h>
#include <CommonMicro.h>
#include <VulkanCommonDefine.h>

class VulkanDevice;
class VulkanImageView;
class VulkanImage : enable_shared_from_this<VulkanImage>
{
public:
	~VulkanImage()
	{

	};

	static uint32_t GetImageLayerCount(VkImageViewType inType, uint32_t inNumArraySlices);
private:
	
	VulkanImage()
	{

	};

	DefineMemberWithGetter(VkImage, Img);
	//DefineMemberWithGetter(VulkanImageView, ImgView);
};


class VulkanImageView : enable_shared_from_this<VulkanImageView>
{
public:
	~VulkanImageView()
	{

	};

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
};
