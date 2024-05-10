#include "VulkanRHI/public/VulkanSwapChain.h"
#include "VulkanRHI/public/VulkanDevice.h"
#include "VulkanRHI/public/VulkanImage.h"
#include "VulkanRHI/public/VulkanCommandBuffer.h"

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
	formats.resize(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(inDevice->GetGpu(), inSurfaceKHR, &formatCount, formats.data());
	//获取期望的Surface Format
	VkSurfaceFormatKHR desiredSurfaceFormat;
	desiredSurfaceFormat.colorSpace = VkColorSpaceKHR::VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	desiredSurfaceFormat.format = VkFormat::VK_FORMAT_B8G8R8A8_SRGB;

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

shared_ptr<VulkanSwapChain> VulkanSwapChain::Create(shared_ptr<VulkanDevice> inDevice, VkSurfaceKHR inSurface, VkSurfaceCapabilitiesKHR inSurfaceCapabilities, VkPresentModeKHR inPresentMode, VkSurfaceFormatKHR inSurfaceFormat)
{
	shared_ptr<VulkanSwapChain> newVulkanSwapChain(new VulkanSwapChain(inDevice, inSurface, inSurfaceCapabilities, inPresentMode, inSurfaceFormat));
	newVulkanSwapChain->InitVkSwapChain();
	return newVulkanSwapChain;
};

void VulkanSwapChain::InitVkSwapChain()
{
	VkSwapchainCreateInfoKHR swapchainCretaeInfo;

	ZeroVulkanStruct(swapchainCretaeInfo, VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR);

	swapchainCretaeInfo.surface = mSurface;
	swapchainCretaeInfo.presentMode = mPresentMode;
	swapchainCretaeInfo.imageFormat = mSurfaceFormat.format;
	swapchainCretaeInfo.imageColorSpace = mSurfaceFormat.colorSpace;
	//swapchainCretaeInfo.imageExtent = mSurfaceCapabilities.minImageExtent;
	swapchainCretaeInfo.imageExtent = mSurfaceCapabilities.maxImageExtent;
	//后面需要考虑显卡是否支持，在选择physicaldevice的时候需要考虑capabilities
	swapchainCretaeInfo.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;;
	swapchainCretaeInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCretaeInfo.minImageCount = mSurfaceCapabilities.minImageCount;
	swapchainCretaeInfo.imageArrayLayers = mSurfaceCapabilities.maxImageArrayLayers;
	swapchainCretaeInfo.preTransform = mSurfaceCapabilities.currentTransform;//这个应该是屏幕旋转的时候使用？
	swapchainCretaeInfo.clipped = true; 
	swapchainCretaeInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;//窗口和系统其它窗口的混合模式
	swapchainCretaeInfo.flags = 0;// VkSwapchainCreateFlagBitsKHR::VK_SWAPCHAIN_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT_KHR;
	swapchainCretaeInfo.queueFamilyIndexCount;
	swapchainCretaeInfo.pQueueFamilyIndices;
	swapchainCretaeInfo.surface = mSurface;
	vkCreateSwapchainKHR(mDevice->GetDevice(), &swapchainCretaeInfo, nullptr, &mVkSwapChain);
	
	TArray(VkImage) imgs;
	uint32_t swapChainImgCount = 0;
	vkGetSwapchainImagesKHR(mDevice->GetDevice(), mVkSwapChain, &swapChainImgCount, nullptr);
	imgs.resize(swapChainImgCount);
	vkGetSwapchainImagesKHR(mDevice->GetDevice(), mVkSwapChain, &swapChainImgCount, imgs.data());
	for (auto& image : imgs)
	{
		TextureDesc swapChainImgDesc = TextureDesc::Create2D({ int(swapchainCretaeInfo.imageExtent.width), int(swapchainCretaeInfo.imageExtent.height) },
			EPixelFormat::PF_Unknown,
			{ 0, 0, 0, 0,EClearBinding::EColorBound },
			ETextureCreateFlags::TCF_Presentable,
			1, 1);
		mImages.push_back(image);
		shared_ptr<VulkanImageView> imageView = VulkanImageView::Create(swapChainImgDesc, mDevice, image, swapchainCretaeInfo.imageUsage);
		mImageViews.push_back(imageView);
		//创建信号量
		if (mUseSemaphore)
		{
			VkSemaphoreCreateInfo semaphoreCreateInfo;
			semaphoreCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
			ZeroVulkanStruct(semaphoreCreateInfo, VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO);
			VkSemaphore semaphore;
			vkCreateSemaphore(mDevice->GetDevice(), &semaphoreCreateInfo, nullptr, &semaphore);
			mSemaphores.push_back(semaphore);
		}
		else
		{
			VkFenceCreateInfo fenceCreateInfo;
			fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
			ZeroVulkanStruct(fenceCreateInfo, VK_STRUCTURE_TYPE_FENCE_CREATE_INFO);
			VkFence fence;
			vkCreateFence(mDevice->GetDevice(), &fenceCreateInfo, nullptr, &fence);
		}
	}
	mPresentID = 0;
}

void VulkanSwapChain::AcquireNextImage(VkSemaphore* outSemaphore)
{
	uint32_t imageIndex = 0;
	VkDevice device = mDevice->GetDevice();
	const uint32_t prevSemaphoreIndex = mCurSemaphoreIndex;
	VkResult result = VkResult::VK_SUCCESS;
	if (mUseSemaphore)
	{
		mCurSemaphoreIndex = (mCurSemaphoreIndex + 1) % mSemaphores.size();
		result = vkAcquireNextImageKHR(device, mVkSwapChain, 0xffffffffffffffff, mSemaphores[mCurSemaphoreIndex], VK_NULL_HANDLE, &imageIndex);
	}
	else
	{
		mCurSemaphoreIndex = (mCurSemaphoreIndex + 1) % mFences.size();
		result = vkAcquireNextImageKHR(device, mVkSwapChain, 0xffffffffffffffff, VK_NULL_HANDLE, mFences[mCurSemaphoreIndex], &imageIndex);
	}
	
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		mCurSemaphoreIndex = prevSemaphoreIndex;
	}
	else if (result == VK_ERROR_SURFACE_LOST_KHR)
	{
		mCurSemaphoreIndex = prevSemaphoreIndex;
	}
	mCurBackBufferIndex = imageIndex;
	outSemaphore = &mSemaphores[mCurSemaphoreIndex];
}


void VulkanSwapChain::Present(VkSemaphore inWaitSemaphore)
{
	//vkResetFences(mDevice, 1, &());
	TArray(VkResult) presentResultes;
	presentResultes.resize(1);
	VkPresentInfoKHR presentInfo;
	ZeroVulkanStruct(presentInfo, VK_STRUCTURE_TYPE_PRESENT_INFO_KHR);
	presentInfo.pImageIndices = &mCurBackBufferIndex;
	presentInfo.waitSemaphoreCount = 0;
	if (inWaitSemaphore)
	{
		presentInfo.waitSemaphoreCount = 1;

	}
	presentInfo.pWaitSemaphores = &inWaitSemaphore;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &mVkSwapChain;
	presentInfo.pResults = presentResultes.data();

	auto GetPresentQueueFamily = [&]()
	{
		if (mDevice->GetQueues().PresentQueue.has_value())
		{
			return	mDevice->GetQueues().PresentQueue;
		}
		return	mDevice->GetQueues().PresentQueue;
	};
	shared_ptr<VulkanQueue> presentQueue= GetPresentQueueFamily().value();
	VkResult presentResult = vkQueuePresentKHR(presentQueue->GetVkQueueRef(), &presentInfo);
}


VulkanSwapChain::~VulkanSwapChain()
{
	/*VkDevice device = m_Device->GetInstanceHandle();

	for (int32 index = 0; index < m_ImageAcquiredSemaphore.size(); ++index) {
		vkDestroySemaphore(m_Device->GetInstanceHandle(), m_ImageAcquiredSemaphore[index], VULKAN_CPU_ALLOCATOR);
	}

	vkDestroySwapchainKHR(device, m_SwapChain, VULKAN_CPU_ALLOCATOR);
	vkDestroySurfaceKHR(m_Instance, m_Surface, VULKAN_CPU_ALLOCATOR);*/
}