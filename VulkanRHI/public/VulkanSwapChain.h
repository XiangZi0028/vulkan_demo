#pragma once
#include "CommonMicro.h"
#include <vulkan/vulkan.h>
#include "CommonMicro.h"
#include "VulkanCommonDefine.h"

class VulkanDevice;
class VulkanSwapChain : public enable_shared_from_this<VulkanSwapChain>
{
public:

	~VulkanSwapChain()
	{

	};
	static shared_ptr<VulkanSwapChain> Create(VkSurfaceKHR inSurface, VkSurfaceCapabilitiesKHR inSurfaceCapabilities, VkPresentModeKHR inPresentMode, VkSurfaceFormatKHR inSurfaceFormat);

	static VkPresentModeKHR GetDesierdPresentModel(const shared_ptr<VulkanDevice>& inVulkanDevice, VkSurfaceKHR inSurfaceKHR, bool inEnableVsync = false);

	static VkSurfaceFormatKHR GetDesierdSurfaceFormat(const shared_ptr<VulkanDevice>& inVulkanDevice, VkSurfaceKHR inSurfaceKHR);

	static VkSurfaceCapabilitiesKHR GetSurfaceCapabilities(const shared_ptr<VulkanDevice>& inVulkanDevice, VkSurfaceKHR inSurfaceKHR);
private:
	VulkanSwapChain(VkSurfaceKHR inSurface, VkSurfaceCapabilitiesKHR inSurfaceCapabilities, VkPresentModeKHR inPresentMode, VkSurfaceFormatKHR inSurfaceFormat)
		: mSurface(inSurface)
		, mSurfaceCapabilities(inSurfaceCapabilities)
		, mPresentMode(inPresentMode)
		, mSurfaceFormat(inSurfaceFormat)
	{

	};
	void InitVkSwapChain();
private:
	VkPresentModeKHR mPresentMode;
	VkSurfaceFormatKHR mSurfaceFormat;
	VkSurfaceCapabilitiesKHR mSurfaceCapabilities;
	DefineMemberWithGetter(VkSurfaceKHR, Surface);
	DefineMemberWithGetter(VkSwapchainKHR, VkSwapChain);
	DefineMemberWithGetter(shared_ptr<VulkanDevice>, Device);
	TArray(VkImage) mBackendBuffer;
	TArray(VkImageView) mBackendBufferView;
};

