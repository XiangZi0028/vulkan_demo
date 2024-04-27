#pragma once
#include "CommonMicro.h"
#include "VulkanCommandBuffer.h"
#include <vulkan/vulkan.h>


class VulkanCommandBuffer : public enable_shared_from_this<VulkanCommandBuffer>
{
public:
	~VulkanCommandBuffer();

	shared_ptr<VulkanCommandBuffer> Create();

private:
	VulkanCommandBuffer() {};

};
