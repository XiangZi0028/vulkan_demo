#include <VulkanRHI/public/VulkanBufferResource.h>
#include <VulkanRHI/public/VulkanDevice.h>
shared_ptr<VulkanBufferResource> VulkanBufferResource::Create(shared_ptr<VulkanDevice> inVulkanDevice, VkBufferUsageFlags inBufferUsageFlags, VkMemoryPropertyFlags inMemoryPropertyFlags, VkDeviceSize inBufferSize, const void *inData)
{
	shared_ptr<VulkanBufferResource> newBufferResource(new VulkanBufferResource(inVulkanDevice->GetDevice()));
	VkDevice device = newBufferResource->GetVkDeviceRef();
	//CreateBuffer
	VkBufferCreateInfo bufferCreateInfo;
	ZeroVulkanStruct(bufferCreateInfo, VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO);
	bufferCreateInfo.usage = inBufferUsageFlags;
	bufferCreateInfo.size = inBufferSize;
	if (vkCreateBuffer(device, &bufferCreateInfo, nullptr, &(newBufferResource->GetVkBufferRef())) != VkResult::VK_SUCCESS)
	{
		return nullptr;
	}
	//Get MemoryRequirements
	VkMemoryRequirements memroyRequirements = {};
	vkGetBufferMemoryRequirements(device, newBufferResource->GetVkBufferRef(), &memroyRequirements);

	VkPhysicalDeviceMemoryProperties gpuMemoryProperties;
	// The returned structure contains memoryTypes and memoryHeaps members
	vkGetPhysicalDeviceMemoryProperties(inVulkanDevice->GetGpu(), &gpuMemoryProperties);
	// Find available memory type Index
	int memoryTypeIndex = -1;
	for (uint32_t Index = 0; Index < gpuMemoryProperties.memoryTypeCount; Index++) {
		if ((memroyRequirements.memoryTypeBits & (1 << Index)) && (gpuMemoryProperties.memoryTypes[Index].propertyFlags  & inMemoryPropertyFlags) == inMemoryPropertyFlags) {
			memoryTypeIndex = Index;
			break;
		}
	}
	if (memoryTypeIndex == -1)
	{
		throw std::runtime_error("failed to find suitable memory type!");
	}
	//Allocated memory
	VkMemoryAllocateInfo memoryAllocatInfo;
	ZeroVulkanStruct(memoryAllocatInfo, VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO);
	memoryAllocatInfo.allocationSize = memroyRequirements.size;
	memoryAllocatInfo.memoryTypeIndex = memoryTypeIndex;
	vkAllocateMemory(device, &memoryAllocatInfo, nullptr, &(newBufferResource->GetVkDeviceMemoryRef()));

	newBufferResource->SetAlignment(memroyRequirements.alignment);
	newBufferResource->SetBufferUsageFlags(inBufferUsageFlags);
	newBufferResource->SetBufferSize(inBufferSize);
	newBufferResource->SetAlocatedMemorySize(memroyRequirements.size);
	newBufferResource->SetMemoryPropertyFlags(inMemoryPropertyFlags);

	//Bind buffer and memory
	newBufferResource->Bind();

	//Write data to memory
	if (inData)
	{
		newBufferResource->Map();
		memcpy(newBufferResource->GetMapped(), inData, inBufferSize);
		if ((inMemoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0)
		{
			newBufferResource->Flush();
		}
		newBufferResource->UnMap();
	}
	return newBufferResource;
}

void VulkanBufferResource::Bind(VkDeviceSize inOffset)
{
	vkBindBufferMemory(mVkDevice, mVkBuffer, mVkDeviceMemory, inOffset);
}

VkResult VulkanBufferResource::Map(VkDeviceSize inSize, VkDeviceSize inOffset)
{
	if (mMapped)
	{
		return VK_SUCCESS;
	}
	return vkMapMemory(mVkDevice, mVkDeviceMemory, inOffset, inSize, 0, &mMapped);
}

void VulkanBufferResource::UnMap()
{
	if (!mMapped)
	{
		return;
	}
	vkUnmapMemory(mVkDevice, mVkDeviceMemory);
	mMapped = nullptr;
}

VkResult VulkanBufferResource::Flush(VkDeviceSize inSize, VkDeviceSize inOffset)
{
	VkMappedMemoryRange mappedRange;
	ZeroVulkanStruct(mappedRange, VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE);
	mappedRange.memory = mVkDeviceMemory;
	mappedRange.offset = inOffset;
	mappedRange.size = inSize;
	return vkFlushMappedMemoryRanges(mVkDevice, 1, &mappedRange);
}

VkResult VulkanBufferResource::Invalidate(VkDeviceSize inSize, VkDeviceSize inOffset)
{
	VkMappedMemoryRange mappedRange;
	ZeroVulkanStruct(mappedRange, VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE);
	mappedRange.memory = mVkDeviceMemory;
	mappedRange.offset = inOffset;
	mappedRange.size = inSize;
	return vkInvalidateMappedMemoryRanges(mVkDevice, 1, &mappedRange);
}

VulkanBufferResource::~VulkanBufferResource()
{
	//Invalidate();
	if (mVkBuffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(mVkDevice, mVkBuffer, nullptr);
	}
	if (mVkDeviceMemory)
	{
		vkFreeMemory(mVkDevice, mVkDeviceMemory, nullptr);
	}
}