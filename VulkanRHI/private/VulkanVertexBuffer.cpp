#include <VulkanRHI/public/VulkanDevice.h>
#include <VulkanRHI/public/VulkanVertexBuffer.h>
#include <VulkanRHI/public/VulkanBufferResource.h>
#include <VulkanRHI/public/VulkanCommandBuffer.h>
#include <VulkanRHI/public/CommonMicro.h>
#include <VulkanRHI/public/VulkanCommonDefine.h>
#include <vulkan/vulkan.h>

//Index Buffer
shared_ptr<VulkanIndexBuffer> VulkanIndexBuffer::Create(shared_ptr<VulkanDevice>& inVulkanDevice, shared_ptr<VulkanCommandBuffer>& inCommandBuffer, const TArray(uint32_t)& inIndicsesData)
{
	shared_ptr<VulkanIndexBuffer> newVertexBuffer(new VulkanIndexBuffer(inVulkanDevice, inIndicsesData.size(), VkIndexType::VK_INDEX_TYPE_UINT32));
	newVertexBuffer->UploadBuffer(inCommandBuffer, inIndicsesData);
	return newVertexBuffer;
}

shared_ptr<VulkanIndexBuffer> VulkanIndexBuffer::Create(shared_ptr<VulkanDevice>& inVulkanDevice, shared_ptr<VulkanCommandBuffer>& inCommandBuffer, const TArray(uint16_t)& inIndicsesData)
{
	shared_ptr<VulkanIndexBuffer> newVertexBuffer(new VulkanIndexBuffer(inVulkanDevice, inIndicsesData.size(), VkIndexType::VK_INDEX_TYPE_UINT16));
	newVertexBuffer->UploadBuffer(inCommandBuffer, inIndicsesData);
	return newVertexBuffer;
}

template<typename DataType>
void VulkanIndexBuffer::UploadBuffer(shared_ptr<VulkanCommandBuffer>& inCommandBuffer, TArray(DataType) inIndicsesData)
{
	shared_ptr<VulkanBufferResource> stagingBuffer = VulkanBufferResource::Create(mDevice,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		inIndicsesData.size() * sizeof(DataType),
		inIndicsesData.data());

	shared_ptr<VulkanBufferResource> mVulkanResourceBuffer = VulkanBufferResource::Create(mDevice,
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		inIndicsesData.size() * sizeof(DataType),
		nullptr );

	SetVulkanResourceBuffer(mVulkanResourceBuffer);
	
	inCommandBuffer->BeginCommandBuffer();
	VkBufferCopy copyRegion = {};
	copyRegion.size = inIndicsesData.size() * sizeof(DataType);
	vkCmdCopyBuffer(inCommandBuffer->GetCommandBufferRef(), stagingBuffer->GetVkBufferRef(), mVulkanResourceBuffer->GetVkBufferRef(), 1, &copyRegion);
	inCommandBuffer->EndCommandBuffer();
	inCommandBuffer->SubmitCommandBuffer();

	stagingBuffer.reset();
}

VulkanIndexBuffer::VulkanIndexBuffer(shared_ptr<VulkanDevice> inDevice, uint32_t inIndexCount, VkIndexType inIndexType)
	: mDevice(inDevice)
	, mIndexCount(inIndexCount)
	, mVkIndexType(inIndexType)
{

}



//Vertex Buffer
shared_ptr<VulkanVertexBuffer> VulkanVertexBuffer::Create(shared_ptr<VulkanDevice> inDevice, shared_ptr<VulkanCommandBuffer> inCommandBuffer, const TArray(float)& inVertexData, const TArray(EVertexIAType)& inIATypes)
{
	uint32_t singleVertexIASize = GetVertexIATypesTotalSize(inIATypes);
	uint32_t VertexDataSize = inVertexData.size() * sizeof(float);
	if (singleVertexIASize == 0 || VertexDataSize == 0)
	{
		return nullptr;
	}
	if ((VertexDataSize % singleVertexIASize) != 0)
	{
		throw std::runtime_error("The entered vertex data size does not match the vertex attribute!");
	}
	shared_ptr<VulkanVertexBuffer> newVertexBuffer(new VulkanVertexBuffer(inDevice, inIATypes, singleVertexIASize, VertexDataSize));

	shared_ptr<VulkanBufferResource> stagingBuffer = VulkanBufferResource::Create(inDevice,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VertexDataSize,
		inVertexData.data());

	shared_ptr<VulkanBufferResource> mVulkanResourceBuffer = VulkanBufferResource::Create(inDevice,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VertexDataSize,
		nullptr);
	newVertexBuffer->SetVulkanResourceBuffer(mVulkanResourceBuffer);
	newVertexBuffer->SetupVertexInputBindingDesc();
	newVertexBuffer->SetupVertexIADesc();
	inCommandBuffer->BeginCommandBuffer();
	VkBufferCopy copyRegion = {};
	copyRegion.size = VertexDataSize;
	vkCmdCopyBuffer(inCommandBuffer->GetCommandBufferRef(), stagingBuffer->GetVkBufferRef(), mVulkanResourceBuffer->GetVkBufferRef(), 1, &copyRegion);
	inCommandBuffer->EndCommandBuffer();
	inCommandBuffer->SubmitCommandBuffer();

	stagingBuffer.reset();

	return newVertexBuffer;
}

uint32_t VulkanVertexBuffer::GetVertexIATypeSize(const EVertexIAType& inIAType)
{
	switch (inIAType)
	{
		case IA_Position : { return 3; }
		case IA_UV0:{ return 2; }
		case IA_UV1:{ return 2; }
		case IA_Normal:{ return 3; }
		case IA_Tangent:{ return 4; }
		case IA_Color:{ return 3; }
		case IA_SkinWeight:{ return 4; }
		case IA_SkinIndex:{ return 4; }
		case IA_SkinPack :{ return 3; }
		case IA_InstanceFloat1 :{ return 1; }
		case IA_InstanceFloat2 :{ return 2; }
		case IA_InstanceFloat3:{ return 3; }
		case IA_InstanceFloat4:{ return 4; }
		case IA_Custom0:{ return 4; }
		case IA_Custom1: { return 4; }
		case IA_Custom2:{ return 4; }
		case IA_Custom3:{ return 4; }
		default:{ return 0; }
	}
	return 0;
}

VkFormat VulkanVertexBuffer::GetVertexIATypeFormat(const EVertexIAType& inIAType)
{
	switch (inIAType)
	{
	case IA_Position: { return VK_FORMAT_R32G32B32_SFLOAT; }
		case IA_UV0: { return VK_FORMAT_R32G32_SFLOAT; }
		case IA_UV1: { return VK_FORMAT_R32G32_SFLOAT; }
		case IA_Normal: { return VK_FORMAT_R32G32B32_SFLOAT; }
		case IA_Tangent: { return VK_FORMAT_R32G32B32A32_SFLOAT; }
		case IA_Color: { return VK_FORMAT_R32G32B32_SFLOAT; }
		case IA_SkinWeight: { return VK_FORMAT_R32G32B32A32_SFLOAT; }
		case IA_SkinIndex: { return VK_FORMAT_R32G32B32A32_SFLOAT; }
		case IA_SkinPack: { return VK_FORMAT_R32G32B32_SFLOAT; }
		case IA_InstanceFloat1: { return VK_FORMAT_R32_SFLOAT; }
		case IA_InstanceFloat2: { return VK_FORMAT_R32G32_SFLOAT; }
		case IA_InstanceFloat3: { return VK_FORMAT_R32G32B32_SFLOAT; }
		case IA_InstanceFloat4: { return VK_FORMAT_R32G32B32A32_SFLOAT; }
		case IA_Custom0: { return VK_FORMAT_R32G32B32A32_SFLOAT; }
		case IA_Custom1: { return VK_FORMAT_R32G32B32A32_SFLOAT; }
		case IA_Custom2: { return VK_FORMAT_R32G32B32A32_SFLOAT; }
		case IA_Custom3: { return VK_FORMAT_R32G32B32A32_SFLOAT; }
		default: { return ::VK_FORMAT_UNDEFINED; }
	}
	return VK_FORMAT_UNDEFINED;
}


uint32_t VulkanVertexBuffer::GetVertexIATypesTotalSize(const TArray(EVertexIAType)& inIATypes)
{
	uint32_t totalSize = 0;
	for (auto elementType : inIATypes)
	{
		totalSize += GetVertexIATypeSize(elementType);
	}
	return totalSize * sizeof(float);
}

void VulkanVertexBuffer::SetupVertexIADesc()
{
	uint32_t offset = 0;
	for (int Index = 0; Index < mIATypes.size(); ++Index)
	{
		VkVertexInputAttributeDescription vertexIADesc{};
		vertexIADesc.binding = 0;
		vertexIADesc.location = Index;
		vertexIADesc.format = GetVertexIATypeFormat(mIATypes[Index]);
		vertexIADesc.offset = offset;
		offset += GetVertexIATypeSize(mIATypes[Index]);
		mVertexInputAttributesDesc.push_back(vertexIADesc);
	}
}

void VulkanVertexBuffer::SetupVertexInputBindingDesc()
{
	mVertexInputBindingDesc.binding = 0;
	mVertexInputBindingDesc.inputRate = VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;
	mVertexInputBindingDesc.stride = mTotalDataSize;
}