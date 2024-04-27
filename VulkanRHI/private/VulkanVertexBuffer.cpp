#include<VulkanRHI/public/VulkanVertexBuffer.h>
#include<VulkanRHI/public/VulkanDevice.h>
#include<VulkanRHI/public/VulkanBufferResource.h>
#include<VulkanRHI/public/CommonMicro.h>
#include<vulkan/vulkan.h>

template<typename DataType>
shared_ptr<VulkanVertexBuffer> VulkanVertexBuffer::CreateBuffer(shared_ptr<VulkanDevice>& inVulkanDevice, shared_ptr<VulkanCommandBuffer>& inCommandBuffer, TArray(DataType) inVertexData)
{
	shared_ptr<VulkanVertexBuffer> newVertexBuffer = NEW_SHARED(VulkanVertexBuffer(inVulkanDevice->GetDevice(), inIndices.size(), VkIndexType::VK_INDEX_TYPE_UINT32));
	
	shared_ptr<VulkanBufferResource> stagingBuffer = VulkanBufferResource::Create(inVulkanDevice,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		inVertexData.data() * sizeof(DataType), 
		inVertexData.data()
	);

	shared_ptr<VulkanBufferResource> mVulkanResourceBuffer = VulkanBufferResource::Create(inVulkanDevice,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		inVertexData.data() * sizeof(DataType),
	);
	newVertexBuffer->SetVulkanResourceBuffer(mVulkanResourceBuffer);
	inCommandBuffer->Begin();

	VkBufferCopy copyRegion = {};
	copyRegion.size = vertices.size() * sizeof(DataType);
	vkCmdCopyBuffer(inCommandBuffer->GetCmdBuffer(), stagingBuffer->GetVkBufferRef(), mVulkanResourceBuffer->GetVkBufferRef(), 1, &copyRegion);

	inCommandBuffer->End();
	inCommandBuffer->Submit();

	delete stagingBuffer;

	return newVertexBuffer;
}

VulkanVertexBuffer::VulkanVertexBuffer(VkDevice inDevice, int inIndexCount, VkIndexType inIndexType)
	: mVkDevice(inDevice)
	, mIndexCount(inIndexCount)
	, mVkIndexType(inIndexType)
{

}