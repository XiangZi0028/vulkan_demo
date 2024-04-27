#include <VulkanRHI/public/VulkanCommandBuffer.h>

shared_ptr<VulkanCommandBuffer> Create()
{
	return NEW_SHARED(VulkanCommandBuffer);
}