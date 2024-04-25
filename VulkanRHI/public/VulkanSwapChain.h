#pragma once
#include"CommonMicro.h"
#include<vulkan/vulkan.h>

class VulkanSwapChain : public enable_shared_from_this<VulkanSwapChain>
{
public:
	VulkanSwapChain();
	~VulkanSwapChain();

private:
	TArray(VkImage) mBackendBuffer;
	TArray(VkImageView) mBackendBufferView;
};

