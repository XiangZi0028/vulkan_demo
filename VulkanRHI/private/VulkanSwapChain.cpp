#include "VulkanRHI/public/VulkanSwapChain.h"
#include "VulkanRHI/public/VulkanDevice.h"

VkPresentModeKHR VulkanSwapChain::GetDesierdPresentModel(const shared_ptr<VulkanDevice>& inVulkanDevice, VkSurfaceKHR inSurfaceKHR, bool inEnableVsync)
{
	unsigned int presentModesNum = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(inVulkanDevice->GetGpu(), inSurfaceKHR, &presentModesNum, nullptr);

	TArray(VkPresentModeKHR) presentModes(presentModesNum);
	vkGetPhysicalDeviceSurfacePresentModesKHR(inVulkanDevice->GetGpu(), inSurfaceKHR, &presentModesNum, presentModes.data());

	bool bSupportPresentModeMailbox = false;
	bool bSupportPresentModeFIFO = false;
	bool bSupportPresentModeImmediate = false;

	for (unsigned int Index = 0; Index < presentModesNum; ++Index)
	{
		switch (presentModes[Index])
		{
		case VK_PRESENT_MODE_MAILBOX_KHR:
			bSupportPresentModeMailbox = true;
			break;
		case VK_PRESENT_MODE_IMMEDIATE_KHR:
			bSupportPresentModeImmediate = true;
			break;
		case VK_PRESENT_MODE_FIFO_KHR:
			bSupportPresentModeFIFO = true;
			break;
		default:
			break;
		}
	}

	VkPresentModeKHR enbaledPresentMode = VK_PRESENT_MODE_FIFO_KHR;

	if (!inEnableVsync && bSupportPresentModeImmediate)
	{
		enbaledPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
	}
	else if (bSupportPresentModeMailbox)
	{
		enbaledPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
	}
	else if (bSupportPresentModeFIFO)
	{
		enbaledPresentMode = VK_PRESENT_MODE_FIFO_KHR;
	}
	else
	{
		enbaledPresentMode = presentModes[0];
	}
	return enbaledPresentMode;
}


VkSurfaceFormatKHR VulkanSwapChain::GetDesierdSurfaceFormat(const shared_ptr<VulkanDevice> &inDevice, VkSurfaceKHR inSurfaceKHR)
{
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(inDevice->GetGpu(), inSurfaceKHR, &formatCount, nullptr);
	TArray(VkSurfaceFormatKHR) formats;
	vkGetPhysicalDeviceSurfaceFormatsKHR(inDevice->GetGpu(), inSurfaceKHR, &formatCount, formats.data());
	//获取期望的Surface Format
	VkSurfaceFormatKHR desiredSurfaceFormat;
	desiredSurfaceFormat.colorSpace = VkColorSpaceKHR::VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	desiredSurfaceFormat.format = VkFormat::VK_FORMAT_R8G8B8A8_SRGB;

	for (auto surfaceFormat : formats)
	{
		if (surfaceFormat.colorSpace == desiredSurfaceFormat.colorSpace &&
			surfaceFormat.format == desiredSurfaceFormat.format)
		{
			break;
		}
	}
	return desiredSurfaceFormat;
}

VkSurfaceCapabilitiesKHR VulkanSwapChain::GetSurfaceCapabilities(const shared_ptr<VulkanDevice>& inVulkanDevice, VkSurfaceKHR inSurfaceKHR)
{
	VkSurfaceCapabilitiesKHR SurfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(inVulkanDevice->GetGpu(), inSurfaceKHR, &SurfaceCapabilities);
	return SurfaceCapabilities;
}

shared_ptr<VulkanSwapChain> VulkanSwapChain::Create(VkSurfaceKHR inSurface, VkSurfaceCapabilitiesKHR inSurfaceCapabilities, VkPresentModeKHR inPresentMode, VkSurfaceFormatKHR inSurfaceFormat)
{
	shared_ptr<VulkanSwapChain> newVulkanSwapChain(new VulkanSwapChain(inSurface, inSurfaceCapabilities, inPresentMode, inSurfaceFormat));
	newVulkanSwapChain->InitVkSwapChain();
	return nullptr;
};

void VulkanSwapChain::InitVkSwapChain()
{
	VkSwapchainCreateInfoKHR swapchainCretaeInfo;

	InitializeVkStructture(swapchainCretaeInfo, VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR);

	swapchainCretaeInfo.surface = mSurface;
	swapchainCretaeInfo.presentMode = mPresentMode;
	swapchainCretaeInfo.imageFormat = mSurfaceFormat.format;
	swapchainCretaeInfo.imageColorSpace = mSurfaceFormat.colorSpace;
	swapchainCretaeInfo.imageExtent = mSurfaceCapabilities.minImageExtent;
	//后面需要考虑显卡是否支持，在选择physicaldevice的时候需要考虑capabilities
	swapchainCretaeInfo.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;;
	swapchainCretaeInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCretaeInfo.minImageCount = mSurfaceCapabilities.minImageCount;
	swapchainCretaeInfo.imageArrayLayers = mSurfaceCapabilities.maxImageArrayLayers;
	swapchainCretaeInfo.preTransform = mSurfaceCapabilities.currentTransform;//这个应该是屏幕旋转的时候使用？
	swapchainCretaeInfo.clipped = true; 
	swapchainCretaeInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;//窗口和系统其它窗口的混合模式
	swapchainCretaeInfo.flags = 
	swapchainCretaeInfo.queueFamilyIndexCount;
	swapchainCretaeInfo.pQueueFamilyIndices;
	/*
		VkStructureType                  sType;
		const void*                      pNext;
		VkSwapchainCreateFlagsKHR        flags;
		VkSurfaceKHR                     surface;
		uint32_t                         queueFamilyIndexCount;
		const uint32_t*                  pQueueFamilyIndices;
		VkSurfaceTransformFlagBitsKHR    preTransform;
		VkCompositeAlphaFlagBitsKHR      compositeAlpha;
		VkPresentModeKHR                 presentMode;
		VkBool32                         clipped;
		VkSwapchainKHR                   oldSwapchain;
	*/
	swapchainCretaeInfo.surface = mSurface;

	//vkCreateSwapchainKHR();
}