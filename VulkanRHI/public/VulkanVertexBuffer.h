#pragma once 
#include "CommonMicro.h"
#include "VulkanBufferResource.h"
class VulkanCommandBuffer;
class VulkanDevice;
class VulkanBufferResource;

class VulkanVertexBuffer : public enable_shared_from_this<VulkanVertexBuffer>
{
public:
	~VulkanVertexBuffer() {};

	template<typename DataType>
	shared_ptr<VulkanVertexBuffer> CreateBuffer(shared_ptr<VulkanDevice>& inVulkanDevice, shared_ptr<VulkanCommandBuffer>& inCommandBuffer, TArray(DataType) inVertexData);
private:

	VulkanVertexBuffer::VulkanVertexBuffer(VkDevice inDevice, int inIndexCount, VkIndexType inIndexType);
	
	DefineMemberWithGetterSetter(shared_ptr<VulkanBufferResource>, VulkanResourceBuffer);

	VkDevice mVkDevice;
	int mIndexCount;
	VkIndexType mVkIndexType;
};

