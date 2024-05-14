#pragma once
#include "CommonMicro.h"
#include <vulkan/vulkan.h>
#include "CommonMicro.h"
#include "VulkanCommonDefine.h"

class VulkanDevice;
class VulkanImageView;
class VulkanSwapChain : public enable_shared_from_this<VulkanSwapChain>
{
public:

	~VulkanSwapChain();

	void AcquireNextImage(VkSemaphore& outSemaphore);

	void Present(VkSemaphore inWaitSemaphore);

	static shared_ptr<VulkanSwapChain> Create(shared_ptr<VulkanDevice> inDevice, VkSurfaceKHR inSurface, VkSurfaceCapabilitiesKHR inSurfaceCapabilities, VkPresentModeKHR inPresentMode, VkSurfaceFormatKHR inSurfaceFormat);

	static VkPresentModeKHR GetDesierdPresentModel(const shared_ptr<VulkanDevice>& inVulkanDevice, VkSurfaceKHR inSurfaceKHR, bool inEnableVsync = false);

	static VkSurfaceFormatKHR GetDesierdSurfaceFormat(const shared_ptr<VulkanDevice>& inVulkanDevice, VkSurfaceKHR inSurfaceKHR);

	static VkSurfaceCapabilitiesKHR GetSurfaceCapabilities(const shared_ptr<VulkanDevice>& inVulkanDevice, VkSurfaceKHR inSurfaceKHR);
private:
	VulkanSwapChain(shared_ptr<VulkanDevice> inDevice, VkSurfaceKHR inSurface, VkSurfaceCapabilitiesKHR inSurfaceCapabilities, VkPresentModeKHR inPresentMode, VkSurfaceFormatKHR inSurfaceFormat)
		: mDevice(inDevice)
		, mSurface(inSurface)
		, mSurfaceCapabilities(inSurfaceCapabilities)
		, mPresentMode(inPresentMode)
		, mSurfaceFormat(inSurfaceFormat)
		, mUseSemaphore(true)
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
	DefineMemberWithGetter(TArray(VkImage), Images);
	DefineMemberWithGetter(TArray(shared_ptr<VulkanImageView>), ImageViews)
	
	//采用什么样的同步方式
	DefineMemberWithGetter(bool, UseSemaphore)
	DefineMemberWithGetter(TArray(VkSemaphore), Semaphores);
	DefineMemberWithGetter(TArray(VkFence), Fences);
	DefineMemberDefaultWithGetter(uint32_t, CurBackBufferIndex, 0);
	uint32_t mCurSemaphoreIndex = 0;
	uint32_t mPresentID = 0;
};

