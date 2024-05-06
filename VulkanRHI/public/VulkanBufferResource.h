#pragma once
#include "CommonMicro.h"
#include "VulkanCommonDefine.h"
#include <vulkan/vulkan.h>
class VulkanVertexBuffer;
class VulkanDevice;
class VulkanBufferResource : public enable_shared_from_this<VulkanBufferResource>
{
public:
	~VulkanBufferResource();

	static shared_ptr<VulkanBufferResource> Create(shared_ptr<VulkanDevice> inVulkanDevice, VkBufferUsageFlags inBufferUsageFlags, VkMemoryPropertyFlags inMemoryPropertyFlags, VkDeviceSize inBufferSize, const void* inData);
private:
	VulkanBufferResource(VkDevice inDevice)
	: mVkDevice(inDevice)
	{
	};
private:
	void Bind(VkDeviceSize offset = 0);
	void UnMap();
	VkResult Map(VkDeviceSize inSize = VK_WHOLE_SIZE, VkDeviceSize inOffset = 0);
	VkResult Flush(VkDeviceSize inSize = VK_WHOLE_SIZE, VkDeviceSize inOffset = 0);
	VkResult Invalidate(VkDeviceSize inSize = VK_WHOLE_SIZE, VkDeviceSize inOffset =0);

	DefineMemberWithGetterRefSetter(VkDevice, VkDevice)
	DefineMemberWithGetterRefSetter(VkBuffer, VkBuffer)
	DefineMemberWithGetterRefSetter(VkDeviceMemory, VkDeviceMemory)
	DefineMemberWithGetterRefSetter(VkDescriptorBufferInfo, VkBufferDescriptor)
	DefineMemberWithGetterRefSetter(VkDeviceSize, BufferSize)
	DefineMemberWithGetterRefSetter(VkDeviceSize, AlocatedMemorySize)
	DefineMemberWithGetterRefSetter(VkDeviceSize, Alignment)
	DefineMemberDefaultWithGetter(void*, Mapped, nullptr)
	DefineMemberWithGetterRefSetter(VkBufferUsageFlags, BufferUsageFlags)
	DefineMemberWithGetterRefSetter(VkMemoryPropertyFlags, MemoryPropertyFlags)
};
